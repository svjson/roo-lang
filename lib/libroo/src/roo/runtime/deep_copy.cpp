#include <roo/runtime/deep_copy.h>

#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/host/object.h>
#include <roo/lang/func.h>
#include <roo/runtime/pretty_print.h>
#include <roo/runtime/walk.h>

#include "runtime_transfer.h"

namespace Roo
{
  namespace
  {
    struct DeepCopyChildren
    {
      sptr_val_v children(const sptr_val& parent)
      {
        return Walk::children_of(*parent, Walk::shape_of(*parent));
      }
    };

    std::string bounded_representation(const Value& value)
    {
      try
      {
        std::string representation = Pretty::print(
          value,
          {.indent_width = 2,
           .width = std::numeric_limits<std::size_t>::max(),
           .max_depth = 3,
           .max_elements = 10});
        if (representation.size() > 1024)
        {
          representation = representation.substr(0, 1021) + "...";
        }
        return representation;
      }
      catch (...)
      {
        return "#<unprintable>";
      }
    }

    class RuntimeTransferException : public RooException
    {
     private:
      std::string rejected_type;
      std::string rejected_value;

     public:
      RuntimeTransferException(const std::string& reason, const sptr_val& value)
        : RooException(reason)
        , rejected_type(type_string(*value))
        , rejected_value(bounded_representation(*value))
      {
      }

     protected:
      void append_error_fields(ErrorMapBuilder& builder) const override
      {
        builder.add("rejected-type", Value::string(rejected_type));
        builder.add("rejected-value", Value::string(rejected_value));
      }
    };

    struct PermissiveDeepCopyPolicy
    {
      void inspect(const sptr_val&, Walk::Shape) const {}

      sptr_val copy_leaf(const sptr_val& value) const { return value; }
    };

    struct RuntimeTransferPolicy
    {
      void inspect(const sptr_val& value, Walk::Shape shape) const
      {
        if (value->type == Value::Type::NATIVE_OBJECT &&
            dynamic_cast<RuntimeLocalNativeObject*>(value->nobj().get()))
        {
          throw RuntimeTransferException("Native value belongs to its current runtime.", value);
        }

        if (shape != Walk::Shape::LEAF) return;

        switch (value->type)
        {
        case Value::Type::NIL:
        case Value::Type::NUMBER:
        case Value::Type::STRING:
        case Value::Type::CHAR:
        case Value::Type::BOOL:
        case Value::Type::SYMBOL:
        case Value::Type::KEYWORD:
          return;
        case Value::Type::FUNCTION:
        {
          const sptr_executable executable = value->exec_ptr();
          if (std::dynamic_pointer_cast<UserFunction>(executable) ||
              std::dynamic_pointer_cast<DetachedFunction>(executable) ||
              std::dynamic_pointer_cast<JuxtedFunction>(executable))
          {
            throw RuntimeTransferException("Executable value retains runtime state.", value);
          }
          return;
        }
        case Value::Type::OBJECT:
          throw RuntimeTransferException(
            "AST runtime values cannot be transferred between runtimes.", value);
        default:
          throw RuntimeTransferException("Value cannot be transferred between runtimes.", value);
        }
      }

      sptr_val copy_leaf(const sptr_val& value) const { return value; }
    };

    struct DiagnosticTransferPolicy
    {
      void inspect(const sptr_val&, Walk::Shape) const {}

      sptr_val copy_leaf(const sptr_val& value) const
      {
        try
        {
          RuntimeTransferPolicy transfer_policy;
          transfer_policy.inspect(value, Walk::Shape::LEAF);
          return transfer_policy.copy_leaf(value);
        }
        catch (const RooException&)
        {
          return Value::string(bounded_representation(*value));
        }
      }
    };

    template <typename Policy> class DeepCopyVisitor
    {
     private:
      struct Frame
      {
        sptr_val source;
        Walk::Shape shape;
        sptr_val_v children;
      };

      Policy policy;
      std::unordered_map<const Value*, sptr_val> completed;
      std::unordered_set<const Value*> active;
      std::vector<Frame> frames;
      sptr_val_v copied_roots;

      void append(sptr_val value)
      {
        if (frames.empty())
        {
          copied_roots.push_back(std::move(value));
        }
        else
        {
          frames.back().children.push_back(std::move(value));
        }
      }

     public:
      explicit DeepCopyVisitor(Policy policy)
        : policy(std::move(policy))
      {
      }

      Walk::Control enter(const sptr_val& source)
      {
        if (!source) throw RooException("Cannot deep-copy a null value pointer.");

        if (active.contains(source.get()))
        {
          throw RooException("Cannot deep-copy a cyclic value graph.");
        }

        auto existing = completed.find(source.get());
        if (existing != completed.end())
        {
          append(existing->second);
          return Walk::Control::SKIP;
        }

        const Walk::Shape shape = Walk::shape_of(*source);
        policy.inspect(source, shape);

        if (shape == Walk::Shape::LEAF)
        {
          sptr_val copied = policy.copy_leaf(source);
          completed.emplace(source.get(), copied);
          append(std::move(copied));
          return Walk::Control::SKIP;
        }

        active.insert(source.get());
        frames.push_back({source, shape, {}});
        return Walk::Control::DESCEND;
      }

      void leave(const sptr_val& source)
      {
        Frame frame = std::move(frames.back());
        frames.pop_back();

        sptr_val copied;
        if (frame.shape == Walk::Shape::MAP_SHAPE)
        {
          if (frame.children.size() % 2 != 0)
          {
            throw RooException("Cannot deep-copy a map with an unmatched key or value.");
          }
          copied = Value::map(frame.children);
        }
        else if (source->type == Value::Type::LIST)
        {
          copied = Value::list(frame.children);
        }
        else
        {
          copied = Value::vector(frame.children);
        }

        active.erase(source.get());
        completed.emplace(source.get(), copied);
        append(std::move(copied));
      }

      sptr_val_v take_roots() { return std::move(copied_roots); }
    };

    template <typename Policy>
    sptr_val_v deep_copy_with_policy(const sptr_val_v& roots, Policy policy)
    {
      DeepCopyChildren children;
      DeepCopyVisitor<Policy> visitor(std::move(policy));
      for (const sptr_val& root : roots)
      {
        Walk::walk(root, children, visitor);
      }
      return visitor.take_roots();
    }

    template <typename Policy> sptr_val deep_copy_root(const sptr_val& root, Policy policy)
    {
      return deep_copy_with_policy(sptr_val_v{root}, std::move(policy)).front();
    }
  } // namespace

  sptr_val deep_copy(const sptr_val& root)
  {
    return deep_copy_root(root, PermissiveDeepCopyPolicy{});
  }

  sptr_val_v deep_copy(const sptr_val_v& roots)
  {
    return deep_copy_with_policy(roots, PermissiveDeepCopyPolicy{});
  }

  sptr_val deep_copy_for_runtime_transfer(const sptr_val& root)
  {
    return deep_copy_root(root, RuntimeTransferPolicy{});
  }

  sptr_val_v deep_copy_for_runtime_transfer(const sptr_val_v& roots)
  {
    return deep_copy_with_policy(roots, RuntimeTransferPolicy{});
  }

  sptr_val deep_copy_for_diagnostic_transfer(const sptr_val& root)
  {
    return deep_copy_root(root, DiagnosticTransferPolicy{});
  }
} // namespace Roo
