
#ifndef __SEXP_EXEC_H_
#define __SEXP_EXEC_H_

#include "roo/runtime/lower.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <roo/bind.h>
#include <roo/form.h>
#include <roo/macro_support.h>
#include <roo/namespace.h>
#include <roo/runtime/eval_plan.h>
#include <roo/runtime/node.h>
#include <roo/type.h>

// clang-format off
#define SIG_PREAMBLE Roo::arg_v
#define FN_ARGS1(A1) SIG_PREAMBLE{ Roo::arg A1 }
#define FN_ARGS2(A1,A2) SIG_PREAMBLE{ Roo::arg A1, Roo::arg A2 }
#define FN_ARGS3(A1,A2,A3) SIG_PREAMBLE{ Roo::arg A1, Roo::arg A2, Roo::arg A3  }
#define FN_ARGS4(A1,A2,A3,A4) SIG_PREAMBLE{ Roo::arg A1, Roo::arg A2, Roo::arg A3, Roo::arg A4 }
#define FN_ARGS5(A1,A2,A3,A4,A5) SIG_PREAMBLE{ Roo::arg A1, Roo::arg A2, Roo::arg A3, Roo::arg A4, Roo::arg A5 }
#define FN_ARGS6(A1,A2,A3,A4,A5,A6) SIG_PREAMBLE{ Roo::arg A1, Roo::arg A2, Roo::arg A3, Roo::arg A4, Roo::arg A5, Roo::arg A6 }
#define SELECT_FN_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define FN_ARGS(...) SELECT_FN_MACRO(__VA_ARGS__, FN_ARGS6, FN_ARGS5, FN_ARGS4, FN_ARGS3, FN_ARGS2, FN_ARGS1)(__VA_ARGS__)
#define NO_ARGS SIG_PREAMBLE { }

#define MULTI_SIG_PREAMBLE [&]() { Roo::uptr_sig_v ret;
#define MULTI_SIG_TERMINATE return ret; }()
#define MULTI_SIG_PUSH(SIG) ret.push_back(std::make_unique<Roo::sig> SIG);
#define MULTI_SIG1(SIG1) MULTI_SIG_PREAMBLE MULTI_SIG_PUSH(SIG1) MULTI_SIG_TERMINATE
#define MULTI_SIG2(SIG1, SIG2) MULTI_SIG_PREAMBLE MULTI_SIG_PUSH(SIG1) MULTI_SIG_PUSH(SIG2) MULTI_SIG_TERMINATE
#define MULTI_SIG3(SIG1, SIG2, SIG3) MULTI_SIG_PREAMBLE MULTI_SIG_PUSH(SIG1) MULTI_SIG_PUSH(SIG2) MULTI_SIG_PUSH(SIG3) MULTI_SIG_TERMINATE
#define SELECT_MULTI_SIG(_1, _2, _3, NAME, ...) NAME
#define EXPAND_MULTI_SIG(NAME, ...) NAME(__VA_ARGS__)
#define SIG(SINGLE_SIG_ARG) std::make_unique<Roo::sig> SINGLE_SIG_ARG
#define MULTI_SIG(...) EXPAND_MULTI_SIG(SELECT_MULTI_SIG(__VA_ARGS__, MULTI_SIG3, MULTI_SIG2, MULTI_SIG1), __VA_ARGS__)

#define EXEC_DISPATCH(METHOD_REF) std::bind(METHOD_REF, this, std::placeholders::_1, std::placeholders::_2)

#define EXEC_DECL(DISP_NAME) \
  /*! @brief Native executable implementation */                        \
  Roo::sptr_val DISP_NAME(Roo::Context& ctx, Roo::sptr_val_v& args);

#define EXECNODE_DECL(DISP_NAME) \
  /*! @brief Native special form executable implementation */                        \
  Roo::sptr_val DISP_NAME(Roo::Context& ctx, Roo::SpecialFormNode& snode);

// clang-format on

#define EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                           \
  class EXEC_NAME : public EXEC_TYPE                                                    \
  {                                                                                     \
   public:                                                                              \
    EXEC_NAME();

#define END_CLASS                                                                       \
  };

#define FUNC_MAKE(EXEC_NAME) \
  template <typename... Args> \
  static Roo::sptr_val make(Args&&... args) \
  { \
  return Roo::Value::executable(std::make_shared<EXEC_NAME>(std::forward<Args>(args)...)); \
}

#define SFORM_LOWER_DECL Roo::uptr_exec_node lower_form(Roo::LowerContext& ctx, const Roo::sptr_ast_node& ast_node);

#define SFORM_LOWER_IMPL(SFORM_NAME) \
  Roo::uptr_exec_node SFORM_NAME::lower_form([[maybe_unused]] Roo::LowerContext& ctx, \
                                                [[maybe_unused]] const Roo::sptr_ast_node& ast_node)

#define SFORM_OMIT_LOWER_IMPL(SFORM_NAME) \
  SFORM_LOWER_IMPL(SFORM_NAME) \
  { return nullptr; }

#define EXECNODE_DISPATCH(NAME)                                                                  \
  EXECNODE_DECL(execnode_##NAME)

#define FUNC_DISPATCH(NAME) \
  EXEC_DECL(exec_##NAME)

#define FUNC_DECL1(EXEC_NAME, EXEC_TYPE, DISP_NAME1)                                    \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  FUNC_DISPATCH(DISP_NAME1)                                                             \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

#define FUNC_DECL2(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2)                        \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  FUNC_DISPATCH(DISP_NAME1)                                                             \
  FUNC_DISPATCH(DISP_NAME2)                                                             \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

#define FUNC_DECL3(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2, DISP_NAME3)            \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  FUNC_DISPATCH(DISP_NAME1)                                                             \
  FUNC_DISPATCH(DISP_NAME2)                                                             \
  FUNC_DISPATCH(DISP_NAME3)                                                             \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

#define SFORM_DECL1(EXEC_NAME, EXEC_TYPE, DISP_NAME1)                                   \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  EXECNODE_DISPATCH(DISP_NAME1)                                                         \
  SFORM_LOWER_DECL                                                                      \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

#define SFORM_DECL2(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2)                       \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  EXECNODE_DISPATCH(DISP_NAME1)                                                         \
  EXECNODE_DISPATCH(DISP_NAME2)                                                         \
  SFORM_LOWER_DECL                                                                      \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

#define SFORM_DECL3(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2, DISP_NAME3)           \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  EXECNODE_DISPATCH(DISP_NAME1)                                                         \
  EXECNODE_DISPATCH(DISP_NAME2)                                                         \
  EXECNODE_DISPATCH(DISP_NAME3)                                                         \
  SFORM_LOWER_DECL                                                                      \
  FUNC_MAKE(EXEC_NAME)                                                                  \
  END_CLASS

// clang-format off
#define FUNC(FUNC_NAME, ...) __SELECT_MACRO__3(__VA_ARGS__, FUNC_DECL3, FUNC_DECL2, FUNC_DECL1)(FUNC_NAME, Roo::Function, __VA_ARGS__)

#define FUNC_IMPL(FUNC_NAME, SIGNATURE)                                                 \
  FUNC_NAME::FUNC_NAME()                                                                \
    : Function(SIGNATURE) { }

#define EXEC_BODY(FUNC_NAME, DISP_NAME) Roo::sptr_val FUNC_NAME::DISP_NAME([[maybe_unused]]Roo::Context& ctx, [[maybe_unused]]Roo::sptr_val_v& args)

#define EXECNODE_BODY(FUNC_NAME, DISP_NAME) Roo::sptr_val FUNC_NAME::DISP_NAME([[maybe_unused]]Roo::Context& ctx, [[maybe_unused]]Roo::SpecialFormNode& snode)

#define SPECIAL_FORM_DECL(FUNC_NAME, ...) __SELECT_MACRO__3(__VA_ARGS__, SFORM_DECL3, SFORM_DECL2, SFORM_DECL1)(FUNC_NAME, Roo::SpecialForm, __VA_ARGS__)

// clang-format on

#define MACRO_IMPL(MACRO_NAME, SIGNATURE)                                                          \
  MACRO_NAME::MACRO_NAME()                                                                         \
   : Roo::Macro(SIGNATURE) {}

#define SPECIAL_FORM_IMPL(FORM_NAME, SIGNATURE) \
FORM_NAME::FORM_NAME() \
  : Roo::SpecialForm(SIGNATURE) \
{ \
}

#define EVAL &Roo::Eval::DEFAULT
#define DATA &Roo::Eval::LITERAL
#define NO_EVAL &Roo::Eval::POSTPONE

namespace Roo
{
  extern ROO_API int user_functions_created;
  extern ROO_API int user_functions_ast_created;
  extern ROO_API int user_functions_rtval_created;
  extern ROO_API int user_function_ast_invocations;
  extern ROO_API int user_function_rtval_invocations;
  extern ROO_API int user_function_wrong_path_invocations;

  class Context;
  class Scope;

  typedef bool vararg_mode;

  inline const vararg_mode VARARG = true;

  class Argument
  {
   public:
    const TypeRef* type;
    const EvalMode* eval;
    const bool varargs;

    Argument(const TypeRef*);
    Argument(const TypeRef*, const EvalMode* eval);
    Argument(vararg_mode var, const TypeRef*);
    Argument(vararg_mode var, const TypeRef*, const EvalMode* eval);
    virtual ~Argument() = default;

    bool is_vararg() const;
    bool matches(Value&) const;
    bool matches(const uptr_exec_node&) const;
    CoercionResult coerce(Context& ctx, sptr_val& obj) const;
    /**
     * @brief Query if the argument should be evaluated before being
     * passed to the target executable form, or if it should be passed
     * as-is for the target to handle evaluation itself.
     *
     * DEFAULT arguments are evaluated by the lowered call node. LITERAL and
     * POSTPONE arguments are passed through for the target form/function to
     * interpret.
     */
    bool evalp() const;
    /**
     * @brief Query if the target executable expects the argument as
     * pure literal data.
     */
    bool is_literal() const;
    std::string to_string() const;
  };

  typedef std::function<sptr_val(Context&, SpecialFormNode&)> exec_node_fn;
  typedef std::function<sptr_val(Context&, sptr_val_v&)> exec_val_fn;

  /*!
   * @brief Signature
   */
  class Signature
  {
   protected:
    const std::vector<Argument> arguments;
    exec_node_fn exec_func = nullptr;
    exec_val_fn exec_val = nullptr;

    /*!
     * @brief flag signalling if arguments contains a vararg Argument.
     * Pre-calculated at construction for performance
     */
    bool vararg = false;

    /*!
     * @brief Number of trailing arguments in `arguments` that are optional.
     */
    size_t optional_count = 0;

    /*!
     * @brief True when a rest parameter follows the declared arguments.
     * Allows any number of args beyond the required minimum.
     */
    bool has_rest = false;

   public:
    std::unique_ptr<EvalPattern> eval_pattern;

    Signature(std::vector<Argument> arguments, exec_node_fn);
    Signature(std::vector<Argument> arguments, exec_val_fn);
    Signature(std::vector<Argument> arguments,
              exec_val_fn,
              size_t optional_count,
              bool has_rest);

    const std::vector<Argument>& get_arguments() const;
    size_t get_optional_count() const;
    bool has_rest_parameter() const;

    bool supports_exec_tree() const;
    bool supports_rt_value() const;
    bool is_lazy_eval() const;
    bool matches(const sptr_val_v& args) const;
    bool matches(const uptr_exec_node_v& args) const;
    bool is_literal_arg(std::size_t index) const;
    bool is_literal_arg(std::size_t index, std::size_t n) const;
    bool is_eval_arg(std::size_t index, std::size_t n) const;
    bool is_arg_pattern(std::size_t index) const;
    bool should_eval_arg(std::size_t index) const;

    /*
     * @brief Attempt to coerce arguments list to fit the Signature by inspecting
     * the signature types for possible conversions
     */
    sptr_val_v coerce_args(Context& ctx, sptr_val_v& args);

    sptr_val invoke(Context& ctx, SpecialFormNode& args);
    sptr_val invoke(Context& ctx, sptr_val_v& args);
    sptr_val invoke_matched(Context& ctx, sptr_val_v& args);
    std::string to_string() const;
  };

  /*
   * @brief Base class for all executable Roo objects
   */
  class Executable
  {
   protected:
    Form type;
    std::vector<std::unique_ptr<Signature>> signatures;

   public:
    Executable(Form type, std::unique_ptr<Signature> signature);
    Executable(Form type, std::vector<std::unique_ptr<Signature>> signatures);
    virtual ~Executable() = default;

    Form get_type() const;
    bool supports_exec_tree() const;

    bool operator==(const Executable& other) const;

    const std::vector<std::unique_ptr<Signature>>& get_signatures() const;

    Signature* get_signature(Context& ctx, uptr_exec_node_v& args);

    bool requires_late_binding(CallNode& args);

    virtual Roo::sptr_val execute(Context& ctx, sptr_val_v& args);
    virtual std::string to_string(int depth = -1) const = 0;

    friend class HostTypeRef;
  };

  typedef Signature sig;
  typedef std::unique_ptr<Signature> uptr_sig;
  typedef std::vector<std::unique_ptr<Signature>> uptr_sig_v;
  typedef Argument arg;
  typedef std::vector<Argument> arg_v;

  class Function : public Executable
  {
   public:
    Function(std::unique_ptr<Signature> signature);
    Function(std::vector<std::unique_ptr<Signature>> signatures);

    std::string to_string(int depth = -1) const override;
  };

  class DetachedFunction : public Function
  {
    std::shared_ptr<Context> ctx;
    std::shared_ptr<Function> fun;
    sptr_val_v bound_args;

    std::vector<std::unique_ptr<Signature>> make_detached_signature(Function& fn);

   public:
    DetachedFunction(std::shared_ptr<Context> ctx,
                     std::shared_ptr<Function>& function,
                     sptr_val_v bounds_args = {});

    static std::shared_ptr<DetachedFunction> make_detached(Context& ctx,
                                                           sptr_executable fun_obj,
                                                           sptr_val_v bound_args = {});

    const sptr_val_v get_bound_arguments() const;

    sptr_val execute(Context&, sptr_val_v& args) override;
    sptr_val execute_bound(sptr_val_v& args);
    sptr_val dispatch_detached(Context&, sptr_val_v& args);
  };

  /*!
   * @brief A user defined function, ie a function defined at run-time using
   * the defun-macro.
   */
  class UserFunction : public Function
  {
   private:
    const std::string name;
    /*!
     * @brief The namespace in which the function is defined. When execution occurs,
     * the Context will effectively switch temporarily to this namespace so that any
     * bindings and aliased namespaces are available
     *
     * Should possibly be ubiquitous for all executables and reside in the abstract
     * Executable base class
     */
    const std::string home_ns;
    std::vector<std::unique_ptr<LexicalBinding>> arg_binding;
    uptr_exec_node_v uptr_body;
    ptr_exec_node_v body;
    size_t required_count = 0;
    size_t optional_count = 0;
    std::unique_ptr<RestBinding> rest_binding;

   public:
    UserFunction(const std::string& name,
                 const std::string& home_ns,
                 arg_v,
                 std::vector<std::unique_ptr<LexicalBinding>>& arg_bindings,
                 uptr_exec_node_v&& body,
                 size_t optional_count = 0,
                 std::unique_ptr<RestBinding> rest_binding = nullptr);

    const std::vector<std::unique_ptr<LexicalBinding>>& get_argument_bindings() const;
    const uptr_exec_node_v& get_body() const;

    std::string to_string(int depth = -1) const override;

    sptr_val execute(Roo::Context& ctx, sptr_val_v& args) override;
    sptr_val exec_body(Roo::Context& ctx, sptr_val_v& args);
  };

  class Macro : public Executable
  {
   public:
    Macro(std::unique_ptr<Signature> signature);
    Macro(Roo::uptr_sig_v signatures);

    std::string to_string(int depth = -1) const override;
  };

  class SpecialForm : public Executable
  {
   public:
    SpecialForm(std::unique_ptr<Signature> signature);
    SpecialForm(Roo::uptr_sig_v signatures);

    std::string to_string(int depth = -1) const override;

    virtual Roo::uptr_exec_node lower_form(Roo::LowerContext& ctx,
                                           const Roo ::sptr_ast_node& ast_node) = 0;

    sptr_val exec_node(Context& ctx, SpecialFormNode& node) const;
  };

  std::shared_ptr<UserFunction> create_function(const std::string& name,
                                                Context& ctx,
                                                const Namespace* home_ns,
                                                AST::ASTNode& arg_vector,
                                                sptr_ast_node_v& body);

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                sptr_val_v& param_form,
                                                ptr_exec_node_v& body);

} // namespace Roo

#endif
