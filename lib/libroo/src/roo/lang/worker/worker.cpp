#include "roo/lang/worker/worker.h"

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/host/schema.h>
#include <roo/runtime.h>
#include <roo/runtime/eval_plan.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>
#include <roo/runtime/node.h>
#include <roo/runtime/value.h>
#include <roo/runtime/worker.h>

namespace Roo
{
  namespace
  {
    class LowerFrame
    {
     private:
      LowerContext& context;

     public:
      explicit LowerFrame(LowerContext& context)
        : context(context)
      {
        context.push({});
      }

      ~LowerFrame() { context.pop(); }
    };

    class PushedContexts
    {
     private:
      Context& context;
      size_t count = 0;

     public:
      explicit PushedContexts(Context& context)
        : context(context)
      {
      }

      ~PushedContexts()
      {
        while (count > 0)
        {
          context.pop_context();
          --count;
        }
      }

      void push(Scope& scope)
      {
        context.push_context(true, scope);
        ++count;
      }
    };

    void append_embedded_values(const sptr_ast_node& node, sptr_val_v& values)
    {
      if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(node.get()))
      {
        values.push_back(wrapper->val);
        return;
      }

      if (auto* sequence = dynamic_cast<AST::Seq*>(node.get()))
      {
        for (const auto& child : sequence->children)
        {
          append_embedded_values(child, values);
        }
      }
    }

    SourceRef localize_source(const SourceRef& source,
                              const SourceMap& source_map,
                              SourceMap& destination_map)
    {
      if (!source.valid()) return {};

      SourceRef localized = source;
      localized.file_id = destination_map.intern_file(source_map.file_name(source.file_id));
      return localized;
    }

    sptr_ast_node copy_ast_for_worker(const sptr_ast_node& node,
                                      const SourceMap& source_map,
                                      SourceMap& destination_map,
                                      const sptr_val_v& embedded_values,
                                      size_t& embedded_index)
    {
      sptr_ast_node copied;
      if (dynamic_cast<AST::RuntimeValueWrapper*>(node.get()))
      {
        if (embedded_index >= embedded_values.size())
        {
          throw RooException("Missing transferred value for captured worker AST.");
        }
        copied =
          std::make_shared<AST::RuntimeValueWrapper>(embedded_values[embedded_index++]);
      }
      else
      {
        switch (node->get_type())
        {
        case Form::VECTOR:
        case Form::LIST:
        case Form::MAP:
        {
          auto& sequence = node->as<AST::Seq>();
          sptr_ast_node_v children;
          children.reserve(sequence.children.size());
          for (const auto& child : sequence.children)
          {
            children.push_back(copy_ast_for_worker(child,
                                                   source_map,
                                                   destination_map,
                                                   embedded_values,
                                                   embedded_index));
          }

          if (node->get_type() == Form::VECTOR)
          {
            copied = std::make_shared<AST::Vector>(children);
          }
          else if (node->get_type() == Form::MAP)
          {
            copied = std::make_shared<AST::Map>(children);
          }
          else
          {
            copied =
              std::make_shared<AST::List>(children, node->as<AST::List>().is_quoted());
          }
          break;
        }
        case Form::BOOLEAN:
        case Form::B_TRUE:
        case Form::B_FALSE:
          copied = std::make_shared<AST::Boolean>(AST::Value<bool>::value_of(*node));
          break;
        case Form::CHAR:
          copied = std::make_shared<AST::Char>(AST::Value<char>::value_of(*node));
          break;
        case Form::KEYWORD:
          copied = std::make_shared<AST::Keyword>(AST::Value<std::string>::value_of(*node));
          break;
        case Form::NIL:
          copied = std::make_shared<AST::Nil>();
          break;
        case Form::NUMBER:
        {
          const auto& number = node->as<AST::Number>();
          switch (number.num_type)
          {
          case AST::NumberType::INT:
            copied = std::make_shared<AST::Number>(number.int_value());
            break;
          case AST::NumberType::LONG:
            copied = std::make_shared<AST::Number>(number.long_value());
            break;
          case AST::NumberType::FLOAT:
            copied = std::make_shared<AST::Number>(number.float_value());
            break;
          }
          break;
        }
        case Form::STRING:
          copied = std::make_shared<AST::String>(AST::Value<std::string>::value_of(*node));
          break;
        case Form::SYMBOL:
          copied = std::make_shared<AST::Symbol>(AST::Value<std::string>::value_of(*node));
          break;
        case Form::QUOTED_SYMBOL:
          copied =
            std::make_shared<AST::QuotedSymbol>(AST::Value<std::string>::value_of(*node));
          break;
        case Form::DISCARD:
          copied = std::make_shared<AST::Discard>();
          break;
        case Form::ANY:
        case Form::FUNCTION:
        case Form::MACRO:
          throw RooException("Unsupported captured worker AST form: " +
                             std::to_string(static_cast<int>(node->get_type())) + ".");
        }
      }

      copied->set_source(localize_source(node->get_source(), source_map, destination_map));
      return copied;
    }

    class ExecuteLetJobFunction : public Function
    {
     private:
      std::vector<std::unique_ptr<LexicalBinding>> bindings;
      sptr_ast_node_v body;
      SourceMap source_map;
      size_t embedded_value_count;

      sptr_val execute_job(Context& ctx, sptr_val_v& arguments)
      {
        const size_t binding_count = bindings.size();
        if (arguments.size() != binding_count + embedded_value_count)
        {
          throw RooException("Invalid transferred worker binding payload.");
        }

        size_t embedded_index = binding_count;
        sptr_ast_node_v localized_body;
        localized_body.reserve(body.size());
        for (const auto& form : body)
        {
          localized_body.push_back(copy_ast_for_worker(form,
                                                       source_map,
                                                       ctx.get_runtime().source_map(),
                                                       arguments,
                                                       embedded_index));
        }
        if (embedded_index != arguments.size())
        {
          throw RooException("Unused transferred value in captured worker AST.");
        }

        LowerContext lower_ctx{&ctx};
        for (const auto& binding : bindings)
        {
          lower_ctx.add_lexical_binding(*binding);
        }

        PushedContexts pushed(ctx);
        for (size_t i = 0; i < binding_count; ++i)
        {
          Scope scope;
          bindings[i]->apply(scope, arguments[i]);
          pushed.push(scope);
        }

        sptr_val result;
        for (const auto& form : localized_body)
        {
          uptr_exec_node node = lower_expr(lower_ctx, form);
          result = exec(ctx, *node);
        }
        return result ? result : Constant::NIL;
      }

     public:
      ExecuteLetJobFunction(std::vector<std::unique_ptr<LexicalBinding>> bindings,
                            sptr_ast_node_v body,
                            SourceMap source_map,
                            size_t embedded_value_count)
        : Function(std::make_unique<Signature>(
            arg_v{arg(VARARG, &Type::ANY)},
            exec_val_fn(std::bind(&ExecuteLetJobFunction::execute_job,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2))))
        , bindings(std::move(bindings))
        , body(std::move(body))
        , source_map(std::move(source_map))
        , embedded_value_count(embedded_value_count)
      {
      }

      std::string to_string(int) const override { return "#<roo.worker/execute-let-job>"; }
    };
  } // namespace

  /** CollectWorkerBangFunction - roo.worker/collect! */
  FUNC_IMPL(CollectWorkerBangFunction,
            SIG((FN_ARGS((&Type::ANY)),
                 EXEC_DISPATCH(&CollectWorkerBangFunction::exec_collect_worker))))

  EXEC_BODY(CollectWorkerBangFunction, exec_collect_worker)
  {
    return ctx.get_runtime().worker_registry().collect(args[0]);
  }

  /** CreateWorkerBangFunction - roo.worker/create! */
  FUNC_IMPL(CreateWorkerBangFunction,
            MULTI_SIG((FN_ARGS((&Type::KEYWORD)),
                       EXEC_DISPATCH(&CreateWorkerBangFunction::exec_create_worker)),
                      (FN_ARGS((&Type::KEYWORD), (&Type::MAP)),
                       EXEC_DISPATCH(&CreateWorkerBangFunction::exec_create_worker))))

  EXEC_BODY(CreateWorkerBangFunction, exec_create_worker)
  {
    WorkerRegistry& registry = ctx.get_runtime().worker_registry();
    if (args.size() == 1)
    {
      registry.create(args[0]->str());
      return Constant::NIL;
    }

    static MapSchema schema({}, {{"environment", &Type::KEYWORD}});
    MapSchema::Inspector options = schema.bind(ctx, *args[1]);
    if (options.contains("environment"))
    {
      registry.create(args[0]->str(), options.val("environment")->str());
    }
    else
    {
      registry.create(args[0]->str());
    }
    return Constant::NIL;
  }

  /** ExecuteLetWorkerBangForm - roo.worker/execute-let! */
  SPECIAL_FORM_IMPL(
    ExecuteLetWorkerBangForm,
    SIG((FN_ARGS((&Type::KEYWORD),
                 (&Type::VECTOR, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                 (VARARG, &Type::ANY, NO_EVAL)),
         EXEC_DISPATCH(&ExecuteLetWorkerBangForm::execnode_execute_let_worker))))

  SFORM_LOWER_IMPL(ExecuteLetWorkerBangForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() < 3)
    {
      throw InvalidFormException(
        "roo.worker/execute-let!: Expected a worker identity and binding vector.");
    }
    if (elements[2]->get_type() != Form::VECTOR)
    {
      throw TypeError("roo.worker/execute-let!: Invalid bind form: " +
                      elements[2]->to_string());
    }

    sptr_ast_node_v& bind_forms = elements[2]->get_children();
    if (bind_forms.size() % 2 != 0)
    {
      throw InvalidFormException("roo.worker/execute-let!: Unmatched binding-value pairs: " +
                                 elements[2]->to_string());
    }

    uptr_exec_node worker_identity = lower_expr(ctx, elements[1]);
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(bind_forms.size() / 2);

    sptr_val_v values;
    values.reserve(bind_forms.size() / 2 + elements.size() - 3);

    LowerFrame frame(ctx);
    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      uptr_exec_node pattern_node = lower_literal(ctx, bind_forms[i]);
      auto& pattern = std::get<LiteralNode>(pattern_node->data);
      auto binding = LexicalBinding::create(pattern);
      values.push_back(pattern.value);
      bindings.emplace_back(std::move(binding), lower_expr(ctx, bind_forms[i + 1]));
      ctx.add_lexical_binding(*bindings.back().first);
    }

    for (size_t i = 3; i < elements.size(); ++i)
    {
      values.push_back(Value::object(elements[i]));
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.push_back(std::move(worker_identity));
    return std::make_unique<ExecNode>(
      SpecialFormNode(this, values, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(ExecuteLetWorkerBangForm, execnode_execute_let_worker)
  {
    if (snode.exec_nodes.size() != 1 || snode.values.size() < snode.bind_forms.size())
    {
      throw RooException("Invalid roo.worker/execute-let! execution node.");
    }

    sptr_val worker_identity = exec(ctx, *snode.exec_nodes.front());
    if (worker_identity->type != Value::Type::KEYWORD)
    {
      throw TypeError("roo.worker/execute-let!: Worker identity must be a Keyword.");
    }

    sptr_val_v binding_values;
    binding_values.reserve(snode.bind_forms.size());
    {
      PushedContexts pushed(ctx);
      for (auto& [binding, value_node] : snode.bind_forms)
      {
        sptr_val value = exec(ctx, *value_node);
        binding_values.push_back(value);

        Scope scope;
        binding->apply(scope, value);
        pushed.push(scope);
      }
    }

    std::vector<std::unique_ptr<LexicalBinding>> bindings;
    bindings.reserve(snode.bind_forms.size());
    for (auto& [binding, value_node] : snode.bind_forms)
    {
      (void)value_node;
      bindings.push_back(binding->clone());
    }

    sptr_ast_node_v body;
    body.reserve(snode.values.size() - snode.bind_forms.size());
    for (size_t i = snode.bind_forms.size(); i < snode.values.size(); ++i)
    {
      if (snode.values[i]->type != Value::Type::OBJECT)
      {
        throw RooException("Invalid roo.worker/execute-let! body node.");
      }
      body.push_back(snode.values[i]->obj());
    }

    sptr_val_v invocation_arguments = binding_values;
    for (const auto& form : body)
    {
      append_embedded_values(form, invocation_arguments);
    }
    const size_t embedded_value_count = invocation_arguments.size() - binding_values.size();

    sptr_val job = Value::executable(
      std::make_shared<ExecuteLetJobFunction>(std::move(bindings),
                                              std::move(body),
                                              ctx.get_runtime().source_map(),
                                              embedded_value_count));
    return ctx.get_runtime().worker_registry().invoke(worker_identity->str(),
                                                      job,
                                                      invocation_arguments);
  }

  /** InvokeWorkerBangFunction - roo.worker/invoke! */
  FUNC_IMPL(InvokeWorkerBangFunction,
            SIG((FN_ARGS((&Type::KEYWORD), (&Type::EXEC), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&InvokeWorkerBangFunction::exec_invoke_worker))))

  EXEC_BODY(InvokeWorkerBangFunction, exec_invoke_worker)
  {
    sptr_val_v invocation_arguments(args.begin() + 2, args.end());
    return ctx.get_runtime().worker_registry().invoke(args[0]->str(),
                                                      args[1],
                                                      invocation_arguments);
  }

  /** PollWorkerBangFunction - roo.worker/poll! */
  FUNC_IMPL(PollWorkerBangFunction,
            SIG((FN_ARGS((&Type::ANY)),
                 EXEC_DISPATCH(&PollWorkerBangFunction::exec_poll_worker))))

  EXEC_BODY(PollWorkerBangFunction, exec_poll_worker)
  {
    Runtime& runtime = ctx.get_runtime();
    sptr_val status;
    switch (runtime.worker_registry().poll(args[0]))
    {
    case WorkerExecutionStatus::QUEUED:
      status = Value::keyword("queued", runtime.keyword_pool());
      break;
    case WorkerExecutionStatus::RUNNING:
      status = Value::keyword("running", runtime.keyword_pool());
      break;
    case WorkerExecutionStatus::SUCCEEDED:
      status = Value::keyword("succeeded", runtime.keyword_pool());
      break;
    case WorkerExecutionStatus::FAILED:
      status = Value::keyword("failed", runtime.keyword_pool());
      break;
    }

    if (!status) throw RooException("Worker execution has an invalid status.");
    return Value::map({Value::keyword("status", runtime.keyword_pool()), std::move(status)});
  }
} // namespace Roo
