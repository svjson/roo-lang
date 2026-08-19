#include "roo/runtime/pretty_print.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include <roo/runtime/value.h>
#include <roo/runtime/walk.h>

namespace Roo::Pretty
{
  PrintNode PrintNode::text(std::string value)
  {
    PrintNode node;
    node.kind = Kind::TEXT;
    node.text_value = std::move(value);
    return node;
  }

  PrintNode PrintNode::concat(std::vector<PrintNode> parts)
  {
    PrintNode node;
    node.kind = Kind::CONCAT;
    node.parts = std::move(parts);
    return node;
  }

  PrintNode PrintNode::nest(int width, PrintNode child)
  {
    PrintNode node;
    node.kind = Kind::NEST;
    node.nest_width = width;
    node.parts.push_back(std::move(child));
    return node;
  }

  PrintNode PrintNode::group(PrintNode child)
  {
    PrintNode node;
    node.kind = Kind::GROUP;
    node.parts.push_back(std::move(child));
    return node;
  }

  PrintNode PrintNode::line()
  {
    PrintNode node;
    node.kind = Kind::LINE;
    return node;
  }

  PrintNode PrintNode::soft_line()
  {
    PrintNode node;
    node.kind = Kind::SOFT_LINE;
    return node;
  }

  PrintNode PrintNode::hard_line()
  {
    PrintNode node;
    node.kind = Kind::HARD_LINE;
    return node;
  }

  PrintNode PrintNode::join(std::vector<PrintNode> nodes, const PrintNode& separator)
  {
    if (nodes.empty())
    {
      return PrintNode::concat({});
    }

    std::vector<PrintNode> parts;
    parts.reserve(nodes.size() * 2 - 1);
    for (std::size_t i = 0; i < nodes.size(); i++)
    {
      if (i > 0) parts.push_back(separator);
      parts.push_back(std::move(nodes[i]));
    }
    return PrintNode::concat(std::move(parts));
  }

  namespace
  {
    enum class Mode
    {
      FLAT,
      BREAK
    };

    /*!
     * @brief Measure `node` as if its enclosing group rendered flat, adding
     * to `used` and aborting (returning false) as soon as `used` exceeds
     * `budget`. A HARD_LINE can never be flat, so it always fails.
     */
    bool fits_flat(const PrintNode& node, std::size_t budget, std::size_t& used)
    {
      switch (node.kind)
      {
      case PrintNode::Kind::TEXT:
        used += node.text_value.size();
        return used <= budget;
      case PrintNode::Kind::LINE:
        used += 1;
        return used <= budget;
      case PrintNode::Kind::SOFT_LINE:
        return used <= budget;
      case PrintNode::Kind::HARD_LINE:
        return false;
      case PrintNode::Kind::NEST:
      case PrintNode::Kind::GROUP:
        return fits_flat(node.parts[0], budget, used);
      case PrintNode::Kind::CONCAT:
        for (const PrintNode& part : node.parts)
        {
          if (!fits_flat(part, budget, used)) return false;
        }
        return true;
      }
      return true;
    }

    bool fits_flat(const PrintNode& node, std::size_t budget)
    {
      std::size_t used = 0;
      return fits_flat(node, budget, used);
    }

    void render_node(const PrintNode& node,
                     std::size_t indent,
                     Mode mode,
                     std::size_t width,
                     std::string& out,
                     std::size_t& column)
    {
      switch (node.kind)
      {
      case PrintNode::Kind::TEXT:
        out += node.text_value;
        column += node.text_value.size();
        break;
      case PrintNode::Kind::CONCAT:
        for (const PrintNode& part : node.parts)
        {
          render_node(part, indent, mode, width, out, column);
        }
        break;
      case PrintNode::Kind::NEST:
        render_node(node.parts[0],
                    indent + static_cast<std::size_t>(std::max(0, node.nest_width)),
                    mode,
                    width,
                    out,
                    column);
        break;
      case PrintNode::Kind::GROUP:
      {
        const std::size_t remaining = column < width ? width - column : 0;
        const Mode child_mode =
          fits_flat(node.parts[0], remaining) ? Mode::FLAT : Mode::BREAK;
        render_node(node.parts[0], indent, child_mode, width, out, column);
        break;
      }
      case PrintNode::Kind::LINE:
        if (mode == Mode::FLAT)
        {
          out.push_back(' ');
          column += 1;
        }
        else
        {
          out.push_back('\n');
          out.append(indent, ' ');
          column = indent;
        }
        break;
      case PrintNode::Kind::SOFT_LINE:
        if (mode == Mode::BREAK)
        {
          out.push_back('\n');
          out.append(indent, ' ');
          column = indent;
        }
        break;
      case PrintNode::Kind::HARD_LINE:
        out.push_back('\n');
        out.append(indent, ' ');
        column = indent;
        break;
      }
    }
  } // namespace

  std::string render(const PrintNode& node, const RenderOptions& options)
  {
    std::string out;
    std::size_t column = 0;
    render_node(node, 0, Mode::BREAK, options.width, out, column);
    return out;
  }

  namespace
  {
    std::string open_bracket(Value::Type type, Walk::Shape shape)
    {
      if (shape == Walk::Shape::MAP_SHAPE) return "{";
      return type == Value::Type::LIST ? "(" : "[";
    }

    std::string close_bracket(Value::Type type, Walk::Shape shape)
    {
      if (shape == Walk::Shape::MAP_SHAPE) return "}";
      return type == Value::Type::LIST ? ")" : "]";
    }

    bool depth_truncated(size_t depth, const PrintOptions& options)
    {
      return options.max_depth >= 0 && depth >= static_cast<size_t>(options.max_depth);
    }

    /*!
     * @brief Wrap `children` (already-rendered PrintNodes for this
     * collection's contents - key/value pairs for MAP_SHAPE, elements for
     * SEQ_SHAPE) in brackets, breaking one item per line when the
     * flattened form doesn't fit the render width and collapsing to a
     * single line otherwise.
     */
    PrintNode build_bracketed(Value::Type type,
                              Walk::Shape shape,
                              std::vector<PrintNode> children,
                              bool truncated,
                              const PrintOptions& options)
    {
      if (shape == Walk::Shape::MAP_SHAPE)
      {
        std::vector<PrintNode> entries;
        entries.reserve((children.size() + 1) / 2);
        for (size_t i = 0; i < children.size(); i += 2)
        {
          if (i + 1 < children.size())
          {
            entries.push_back(PrintNode::concat(
              {std::move(children[i]), PrintNode::text(" "), std::move(children[i + 1])}));
          }
          else
          {
            entries.push_back(std::move(children[i]));
          }
        }
        children = std::move(entries);
      }

      if (truncated)
      {
        children.push_back(PrintNode::text("..."));
      }

      if (children.empty())
      {
        return PrintNode::text(open_bracket(type, shape) + close_bracket(type, shape));
      }

      PrintNode body = PrintNode::join(std::move(children), PrintNode::line());
      return PrintNode::group(PrintNode::concat(
        {PrintNode::text(open_bracket(type, shape)),
         PrintNode::nest(options.indent_width,
                         PrintNode::concat({PrintNode::soft_line(), std::move(body)})),
         PrintNode::soft_line(),
         PrintNode::text(close_bracket(type, shape))}));
    }

    /*!
     * @brief Walk::Visitor that builds a PrintNode bottom-up: each
     * non-leaf node (per Walk::shape_of - MAP/LIST/VECTOR, or a host
     * map/sequence) opens a Frame in enter(), collects its children's
     * already-built PrintNodes as they are visited, and assembles them
     * into its own PrintNode in leave() - a native, allocation-light
     * instantiation of the generic walker with no roo interpreter
     * involvement at all.
     */
    class ValueToPrintNode
    {
     public:
      explicit ValueToPrintNode(const PrintOptions& options)
        : options_(options)
      {
      }

      Walk::Control enter(const Value& node)
      {
        const Walk::Shape shape = Walk::shape_of(node);

        if (!frames_.empty())
        {
          Frame& parent = frames_.back();
          const size_t slot = parent.seen++;
          const size_t effective_index =
            parent.shape == Walk::Shape::MAP_SHAPE ? slot / 2 : slot;
          if (options_.max_elements >= 0 &&
              effective_index >= static_cast<size_t>(options_.max_elements))
          {
            parent.truncated = true;
            return Walk::Control::SKIP;
          }
        }

        if (shape != Walk::Shape::LEAF)
        {
          if (depth_truncated(frames_.size(), options_))
          {
            emit(PrintNode::text(open_bracket(node.type, shape) + "..." +
                                 close_bracket(node.type, shape)));
            return Walk::Control::SKIP;
          }
          frames_.push_back(Frame{node.type, shape, {}, 0, false});
          return Walk::Control::DESCEND;
        }

        emit(PrintNode::text(node.to_string()));
        return Walk::Control::SKIP;
      }

      void leave(const Value&)
      {
        Frame frame = std::move(frames_.back());
        frames_.pop_back();
        emit(build_bracketed(frame.type,
                             frame.shape,
                             std::move(frame.children),
                             frame.truncated,
                             options_));
      }

      PrintNode take_result() { return std::move(*root_result_); }

     private:
      struct Frame
      {
        Value::Type type;
        Walk::Shape shape;
        std::vector<PrintNode> children;
        size_t seen = 0;
        bool truncated = false;
      };

      void emit(PrintNode node)
      {
        if (frames_.empty())
        {
          root_result_ = std::move(node);
        }
        else
        {
          frames_.back().children.push_back(std::move(node));
        }
      }

      const PrintOptions& options_;
      std::vector<Frame> frames_;
      std::optional<PrintNode> root_result_;
    };
  } // namespace

  std::string print(const Value& value, const PrintOptions& options)
  {
    ValueToPrintNode visitor(options);
    Walk::walk(value, visitor);

    return render(visitor.take_result(), RenderOptions{options.width});
  }
} // namespace Roo::Pretty
