
#include "roo/runtime/lower.h"
#include "roo/runtime/node.h"
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/lang/rewrite.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/exec_node.h>
#include <roo/scope.h>

namespace Roo
{
  namespace
  {
    sptr_ast_node thread_first_step(const sptr_ast_node& current, const sptr_ast_node& form)
    {
      if (form->get_type() == Form::LIST)
      {
        sptr_ast_node_v& elements = form->get_children();
        sptr_ast_node_v threaded;
        threaded.reserve(elements.size() + 1);
        threaded.push_back(elements[0]);
        threaded.push_back(current);
        for (size_t i = 1; i < elements.size(); i++)
        {
          threaded.push_back(elements[i]);
        }
        return AST::List::make(threaded);
      }

      return AST::List::make({form, current});
    }
  } // namespace

  /** CallAtForm - roo/@> */
  SPECIAL_FORM_IMPL(CallAtForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL), (&Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CallAtForm::execnode_call_at))))

  SFORM_LOWER_IMPL(CallAtForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() != 3)
    {
      throw RooException("@> expects a target and a call form.");
    }

    const sptr_ast_node& target_form = elements[1];
    const sptr_ast_node& call_form = elements[2];
    if (call_form->get_type() != Form::LIST || call_form->get_children().empty())
    {
      throw TypeError("@> expects a non-empty call form as its second argument.");
    }

    const sptr_ast_node& callee_form = call_form->get_children()[0];
    if (callee_form->get_type() != Form::SYMBOL)
    {
      throw TypeError("@> expects the call head to be a symbol, got: " +
                      callee_form->to_string());
    }

    const std::string callee_name = callee_form->to_string();
    uptr_exec_node_v exec_nodes;

    if (target_form->get_type() == Form::VECTOR)
    {
      const sptr_ast_node_v& target_path = target_form->get_children();
      if (target_path.empty())
      {
        throw TypeError("@> target path must contain a target expression.");
      }

      exec_nodes.reserve(target_path.size() + 1);
      for (const sptr_ast_node& path_form : target_path)
      {
        exec_nodes.push_back(lower_expr(ctx, path_form));
      }
    }
    else
    {
      exec_nodes.reserve(2);
      exec_nodes.push_back(lower_expr(ctx, target_form));
    }

    SymbolBinding callee_binding(callee_name);
    ctx.push({});
    ctx.add_lexical_binding(callee_binding);
    try
    {
      exec_nodes.push_back(lower_expr(ctx, call_form));
    }
    catch (...)
    {
      ctx.pop();
      throw;
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this,
                      {Value::string(callee_name), Value::keyword(callee_name)},
                      std::move(exec_nodes)));
  }

  EXECNODE_BODY(CallAtForm, execnode_call_at)
  {
    sptr_val target = exec(ctx, *snode.exec_nodes.front());

    sptr_val_v path;
    path.reserve(snode.exec_nodes.size() - 2);
    for (size_t i = 1; i + 1 < snode.exec_nodes.size(); i++)
    {
      path.push_back(exec(ctx, *snode.exec_nodes[i]));
    }

    sptr_val container = Dict::get_property_path(target, path);
    sptr_val callable = Dict::get_property(container, snode.values[1]);

    Scope call_scope;
    call_scope.store(snode.values[0]->str(), callable);
    ctx.push_context(true, call_scope);
    try
    {
      sptr_val result = exec(ctx, *snode.exec_nodes.back());
      ctx.pop_context();
      return result;
    }
    catch (...)
    {
      ctx.pop_context();
      throw;
    }
  }

  /** CommentForm - roo/comment */
  SPECIAL_FORM_IMPL(CommentForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CommentForm::execnode_comment))))
  SFORM_LOWER_IMPL(CommentForm)
  {
    return std::make_unique<ExecNode>(Constant::NIL);
  }
  EXECNODE_BODY(CommentForm, execnode_comment)
  {
    return Constant::NIL;
  }

  /** CondThreadFirstForm - roo/cond-> */
  SPECIAL_FORM_IMPL(CondThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CondThreadFirstForm::execnode_cond_thread_first))))

  SFORM_LOWER_IMPL(CondThreadFirstForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2) return lower_literal(Roo::AST::NIL);
    if (elements.size() == 2) return lower_expr(ctx, elements[1]);

    if ((elements.size() - 2) % 2 != 0)
    {
      throw RooException("cond->: Requires pairs of [test form], got " +
                         std::to_string(elements.size() - 2) + " trailing args");
    }

    const std::string current_name = "$cond-thread-current";
    auto current_symbol = AST::Symbol::make(current_name);
    SymbolBinding current_binding(current_name);

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 1);

    exec_nodes.push_back(lower_expr(ctx, elements[1]));

    for (size_t i = 2; i < elements.size(); i += 2)
    {
      sptr_ast_node& test = elements[i];
      sptr_ast_node& form = elements[i + 1];

      exec_nodes.push_back(lower_expr(ctx, test));

      ctx.push({});
      ctx.add_lexical_binding(current_binding);
      try
      {
        exec_nodes.push_back(lower_expr(ctx, thread_first_step(current_symbol, form)));
      }
      catch (...)
      {
        ctx.pop();
        throw;
      }
      ctx.pop();
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, {Value::string(current_name)}, std::move(exec_nodes)));
  }

  EXECNODE_BODY(CondThreadFirstForm, execnode_cond_thread_first)
  {
    sptr_val current = exec(ctx, *snode.exec_nodes[0]);
    const std::string current_name = snode.values[0]->str();

    Scope current_scope;
    current_scope.store(current_name, current);
    ctx.push_context(true, current_scope);

    try
    {
      for (size_t node_idx = 1; node_idx < snode.exec_nodes.size(); node_idx += 2)
      {
        if (Roo::is_truthy(*exec(ctx, *snode.exec_nodes[node_idx])))
        {
          current = exec(ctx, *snode.exec_nodes[node_idx + 1]);
          ctx.current_scope().mutate(current_name, current);
        }
      }
    }
    catch (...)
    {
      ctx.pop_context();
      throw;
    }

    ctx.pop_context();

    return current;
  }

  /** ThreadFirstForm - roo/-> */
  SPECIAL_FORM_IMPL(ThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ThreadFirstForm::execnode_thread_first))))

  SFORM_LOWER_IMPL(ThreadFirstForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() == 1) return lower_literal(Roo::AST::NIL);
    if (elements.size() == 2) return lower_expr(ctx, elements[1]);

    sptr_ast_node current = elements[1];

    for (size_t i = 2; i < elements.size(); i++)
    {
      current = thread_first_step(current, elements[i]);
    }

    return lower_expr(ctx, current);
  }
  EXECNODE_BODY(ThreadFirstForm, execnode_thread_first)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

} // namespace Roo
