
#ifndef __SEXP_EXEC_H_
#define __SEXP_EXEC_H_

#include "form.h"
#include "namespace.h"
#include "runtime/eval_plan.h"
#include "runtime/exec_tree.h"
#include "type.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// clang-format off
#define SIG_PREAMBLE Lisple::arg_v
#define FN_ARGS1(A1) SIG_PREAMBLE{ Lisple::arg A1 }
#define FN_ARGS2(A1,A2) SIG_PREAMBLE{ Lisple::arg A1, Lisple::arg A2 }
#define FN_ARGS3(A1,A2,A3) SIG_PREAMBLE{ Lisple::arg A1, Lisple::arg A2, Lisple::arg A3  }
#define FN_ARGS4(A1,A2,A3,A4) SIG_PREAMBLE{ Lisple::arg A1, Lisple::arg A2, Lisple::arg A3, Lisple::arg A4 }
#define FN_ARGS5(A1,A2,A3,A4,A5) SIG_PREAMBLE{ Lisple::arg A1, Lisple::arg A2, Lisple::arg A3, Lisple::arg A4, Lisple::arg A5 }
#define FN_ARGS6(A1,A2,A3,A4,A5,A6) SIG_PREAMBLE{ Lisple::arg A1, Lisple::arg A2, Lisple::arg A3, Lisple::arg A4, Lisple::arg A5, Lisple::arg A6 }
#define SELECT_FN_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define FN_ARGS(...) SELECT_FN_MACRO(__VA_ARGS__, FN_ARGS6, FN_ARGS5, FN_ARGS4, FN_ARGS3, FN_ARGS2, FN_ARGS1)(__VA_ARGS__)
#define NO_ARGS SIG_PREAMBLE { }

#define MULTI_SIG_PREAMBLE [&]() { Lisple::uptr_sig_v ret;
#define MULTI_SIG_TERMINATE return ret; }()
#define MULTI_SIG_PUSH(SIG) ret.push_back(std::make_unique<Lisple::sig> SIG);
#define MULTI_SIG2(SIG1, SIG2) MULTI_SIG_PREAMBLE MULTI_SIG_PUSH(SIG1) MULTI_SIG_PUSH(SIG2) MULTI_SIG_TERMINATE
#define MULTI_SIG3(SIG1, SIG2, SIG3) MULTI_SIG_PREAMBLE MULTI_SIG_PUSH(SIG1) MULTI_SIG_PUSH(SIG2) MULTI_SIG_PUSH(SIG3) MULTI_SIG_TERMINATE
#define SELECT_MULTI_SIG(_1, _2, _3, NAME, ...) NAME
#define SIG(SINGLE_SIG_ARG) std::make_unique<Lisple::sig> SINGLE_SIG_ARG
#define MULTI_SIG(...) SELECT_MULTI_SIG(__VA_ARGS__, MULTI_SIG3, MULTI_SIG2, MULTI_SIG1)(__VA_ARGS__)

#define LEGACY_DISPATCH(METHOD_REF) std::bind(METHOD_REF, this, std::placeholders::_1, std::placeholders::_2)

#define EXEC_NODE_DISPATCH(METHOD_REF) std::bind(METHOD_REF, this, std::placeholders::_1, std::placeholders::_2)

#define DUAL_DISPATCH(LEGACY_REF, METHOD_REF) LEGACY_DISPATCH(LEGACY_REF), EXEC_NODE_DISPATCH(METHOD_REF)

#define SELECT_EXEC_DISPATCH_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define EXEC_DISPATCH(...) SELECT_EXEC_DISPATCH_MACRO(__VA_ARGS__, DUAL_DISPATCH, LEGACY_DISPATCH)(__VA_ARGS__)


#define DISP_DECL(DISP_NAME) \
  /*! @brief Native executable implementation */                        \
  Lisple::sptr_sobject DISP_NAME(Lisple::Context& ctx, Lisple::sptr_sobject_v& args);

#define EXEC_DECL(DISP_NAME) \
  /*! @brief Native executable implementation */                        \
  Lisple::sptr_rtval DISP_NAME(Lisple::Context& ctx, Lisple::ptr_exec_node_v& args);

// clang-format on

#define EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                           \
  class EXEC_NAME : public EXEC_TYPE                                                    \
  {                                                                                     \
   public:                                                                              \
    EXEC_NAME();

#define END_CLASS                                                                       \
  };

#define DISPATCH(NAME)                                                                  \
  DISP_DECL(inv_##NAME)                                                                 \
  EXEC_DECL(exec_##NAME)

#define EXEC_DECL1(EXEC_NAME, EXEC_TYPE, DISP_NAME1)                                    \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISP_DECL(DISP_NAME1)                                                                 \
  END_CLASS

#define EXEC_DECL2(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2)                        \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISP_DECL(DISP_NAME1)                                                                 \
  DISP_DECL(DISP_NAME2)                                                                 \
  END_CLASS

#define EXEC_DECL3(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2, DISP_NAME3)            \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISP_DECL(DISP_NAME1)                                                                 \
  DISP_DECL(DISP_NAME2)                                                                 \
  DISP_DECL(DISP_NAME3)                                                                 \
  END_CLASS

#define SFORM_DECL1(EXEC_NAME, EXEC_TYPE, DISP_NAME1)                                   \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISPATCH(DISP_NAME1)                                                                  \
  END_CLASS

#define SFORM_DECL2(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2)                       \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISPATCH(DISP_NAME1)                                                                  \
  DISPATCH(DISP_NAME2)                                                                  \
  END_CLASS

#define SFORM_DECL3(EXEC_NAME, EXEC_TYPE, DISP_NAME1, DISP_NAME2, DISP_NAME3)           \
  EXEC_CLASS_DECL(EXEC_TYPE, EXEC_NAME)                                                 \
  DISPATCH(DISP_NAME1)                                                                  \
  DISPATCH(DISP_NAME2)                                                                  \
  DISPATCH(DISP_NAME3)                                                                  \
  END_CLASS

#define SELECT_EXEC_DECL_MACRO(_1, _2, _3, MACRO_NAME, ...) MACRO_NAME

// clang-format off
#define FUNC_DECL(FUNC_NAME, ...) SELECT_EXEC_DECL_MACRO(__VA_ARGS__, EXEC_DECL3, EXEC_DECL2, EXEC_DECL1)(FUNC_NAME, Lisple::Function, __VA_ARGS__)

#define FUNC(FUNC_NAME, ...) SELECT_EXEC_DECL_MACRO(__VA_ARGS__, SFORM_DECL3, SFORM_DECL2, SFORM_DECL1)(FUNC_NAME, Lisple::Function, __VA_ARGS__)

#define FUNC_IMPL(FUNC_NAME, SIGNATURE)                                                 \
  FUNC_NAME::FUNC_NAME()                                                                \
    : Function(SIGNATURE) { }

#define FUNC_BODY(FUNC_NAME, DISP_NAME) Lisple::sptr_sobject FUNC_NAME::DISP_NAME([[maybe_unused]]Lisple::Context& ctx, [[maybe_unused]]Lisple::sptr_sobject_v& args)

#define EXEC_BODY(FUNC_NAME, DISP_NAME) Lisple::sptr_rtval FUNC_NAME::DISP_NAME([[maybe_unused]]Lisple::Context& ctx, [[maybe_unused]]Lisple::ptr_exec_node_v& args)

#define MACRO_DECL(FUNC_NAME, ...) SELECT_EXEC_DECL_MACRO(__VA_ARGS__, EXEC_DECL3, EXEC_DECL2, EXEC_DECL1)(FUNC_NAME, Lisple::Macro, __VA_ARGS__)

#define SPECIAL_FORM_DECL(FUNC_NAME, ...) SELECT_EXEC_DECL_MACRO(__VA_ARGS__, SFORM_DECL3, SFORM_DECL2, SFORM_DECL1)(FUNC_NAME, Lisple::Macro, __VA_ARGS__)

// clang-format on

#define MACRO_SUB_DECL(MACRO_BASE, MACRO_NAME, DISP_NAME)                               \
  class MACRO_NAME : public MACRO_BASE                                                  \
  {                                                                                     \
   public:                                                                              \
    MACRO_NAME();                                                                       \
                                                                                        \
    Lisple::sptr_sobject DISP_NAME(Lisple::Context& ctx, Lisple::sptr_sobject_v& args);

#define MACRO_IMPL(MACRO_NAME, SIGNATURE)                                                          \
  MACRO_NAME::MACRO_NAME()                                                                         \
   : Lisple::Macro(SIGNATURE) {}

#define SPECIAL_FORM_IMPL(FORM_NAME, SIGNATURE) \
FORM_NAME::FORM_NAME() \
  : Lisple::Macro(SIGNATURE) \
{ \
}

#define MACRO_SUB_IMPL(MACRO_BASE, MACRO_NAME, SIGNATURE)                                          \
  MACRO_NAME::MACRO_NAME()                                                                         \
      : MACRO_BASE(SIGNATURE)                                                                      \
  {                                                                                                \
  }

#define MACRO_BODY(FUNC_NAME, DISP_NAME)                                                           \
  Lisple::sptr_sobject FUNC_NAME::DISP_NAME([[maybe_unused]] Lisple::Context& ctx,                 \
                                            [[maybe_unused]] Lisple::sptr_sobject_v& args)

#define EVAL &Lisple::Eval::DEFAULT
#define DATA &Lisple::Eval::LITERAL
#define NO_EVAL &Lisple::Eval::POSTPONE

namespace Lisple
{
  class Context;
  class Scope;

  typedef bool vararg_mode;

  inline const vararg_mode VARARG = true;

  class ArgumentBinding
  {
   public:
    virtual ~ArgumentBinding() = default;

    virtual void apply(Scope& scope, sptr_sobject& arg_val) = 0;

    static std::unique_ptr<ArgumentBinding> create(Object& arg_declaration);
  };

  class NamedArgumentBinding : public ArgumentBinding
  {
    std::string arg_name;

   public:
    NamedArgumentBinding(const std::string& name);

    void apply(Scope& scope, sptr_sobject& arg_val) override;
  };

  class DestructuringArgumentBinding : public ArgumentBinding
  {
    Map binding_form;

   public:
    DestructuringArgumentBinding(const Map& destr_map);

    void apply(Scope& scope, sptr_sobject& arg_val) override;
  };

  class ArrayArgumentBinding : public ArgumentBinding
  {
    Array binding_form;

   public:
    ArrayArgumentBinding(const Lisple::Array& array);

    void apply(Scope& scope, sptr_sobject& arg_val) override;
  };

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
    bool matches(Lisple::Object&) const;
    CoercionResult coerce(Context& ctx, sptr_sobject& obj) const;
    /**
     * @brief Query if the argument should be evaluated before being
     * passed to the target executable form, or if it should be passed
     * as-is for the target to handle evaluation itself.
     *
     * This is for legacy use. DEFAULT = true, LITERAL/POSTPONE = false
     */
    bool evalp() const;
    /**
     * @brief Query if the target executable expects the argument as
     * pure literal data.
     */
    bool is_literal() const;
    std::string to_string() const;
  };

  typedef std::function<std::shared_ptr<Object>(Context&, sptr_sobject_v&)> exec_fn;
  typedef std::function<sptr_rtval(Context&, ptr_exec_node_v&)> exec_node_fn;

  /*!
   * @brief Signature
   */
  class Signature
  {
   protected:
    const std::vector<Argument> arguments;
    exec_fn target_func;
    exec_node_fn exec_func = nullptr;

    /*!
     * @brief flag signalling if arguments contains a vararg Argument.
     * Pre-calculated at construction for performance
     */
    bool vararg = false;

   public:
    std::unique_ptr<EvalPattern> eval_pattern;

    Signature(std::vector<Argument> arguments, exec_fn target_func);
    Signature(std::vector<Argument> arguments, exec_fn target_func, exec_node_fn exec_func);

    const std::vector<Argument>& get_arguments() const;

    bool supports_exec_tree() const;
    bool matches(const sptr_sobject_v& args) const;
    bool is_literal_arg(std::size_t index) const;
    bool is_literal_arg(std::size_t index, std::size_t n) const;
    bool is_eval_arg(std::size_t index, std::size_t n) const;
    bool is_arg_pattern(std::size_t index) const;
    bool should_eval_arg(std::size_t index) const;
    /*
     * @brief Attempt to coerce arguments list to fit the Signature by inspecting
     * the signature types for possible conversions
     */
    sptr_sobject_v coerce_args(Context& ctx, sptr_sobject_v& args);

    sptr_sobject invoke(Context& ctx, sptr_sobject_v& args);
    sptr_rtval invoke(Context& ctx, ptr_exec_node_v& args);
    std::string to_string() const;
  };

  /*
   * @brief Base class for all executable Lisple objects
   */
  class Executable : public Lisple::Object
  {
   protected:
    std::vector<std::unique_ptr<Signature>> signatures;

   public:
    Executable(Form type, std::unique_ptr<Signature> signature);
    Executable(Form type, std::vector<std::unique_ptr<Signature>> signatures);

    bool supports_exec_tree() const;

    bool operator==(const Lisple::Object& other) const override;

    Signature* get_signature(Context& ctx, sptr_sobject_v& args);

    virtual Lisple::sptr_sobject execute(Context& ctx, sptr_sobject_v& args) override;

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

    const std::vector<std::unique_ptr<Signature>>& get_signatures() const;

    std::string to_string(int depth = -1) const override;
  };

  class DetachedFunction : public Function
  {
    std::shared_ptr<Context> ctx;
    std::shared_ptr<Function> fun;
    sptr_sobject_v bound_args;

    std::vector<std::unique_ptr<Signature>> make_detached_signature(Function& fn);

   public:
    DetachedFunction(std::shared_ptr<Context> ctx,
                     std::shared_ptr<Function>& function,
                     sptr_sobject_v bounds_args = {});

    static std::shared_ptr<DetachedFunction> make_detached(Context& ctx,
                                                           std::shared_ptr<Object> fun_obj,
                                                           sptr_sobject_v bound_args = {});

    static std::shared_ptr<DetachedFunction> make_detached(Context& ctx,
                                                           std::shared_ptr<Function> fun,
                                                           sptr_sobject_v bound_args = {});

    const sptr_sobject_v get_bound_arguments() const;

    Lisple::sptr_sobject execute(Context&, sptr_sobject_v& args) override;

    sptr_sobject execute_bound(sptr_sobject_v& args);
    sptr_sobject dispatch_detached(Context&, sptr_sobject_v& args);
  };

  /*!
   * @brief A user defined function, ie a function defined at run-time using
   * the defun-macro.
   */
  class UserFunction : public Function
  {
   private:
    /*!
     * @brief The namespace in which the function is defined. When execution occurs,
     * the Context will effectively switch temporarily to this namespace so that any
     * bindings and aliased namespaces are available
     *
     * Should possibly be ubiquitous for all executables and reside in the abstract
     * Executable base class
     */
    const std::string home_ns;
    std::vector<std::unique_ptr<ArgumentBinding>> arg_bindings;
    uptr_exec_node_v uptr_body;
    ptr_exec_node_v body;

   public:
    UserFunction(const std::string& home_ns,
                 arg_v,
                 std::vector<std::unique_ptr<ArgumentBinding>>& arg_bindings,
                 uptr_exec_node_v&& body);

    UserFunction(const std::string& home_ns,
                 Lisple::arg_v,
                 std::vector<std::unique_ptr<ArgumentBinding>>& arg_bindings,
                 sptr_sobject_v& body);

    const std::vector<std::unique_ptr<ArgumentBinding>>& get_argument_bindings() const;
    const uptr_exec_node_v& get_body() const;

    sptr_sobject exec_body(Lisple::Context& ctx, sptr_sobject_v& args);
  };

  class Macro : public Executable
  {
   public:
    Macro(std::unique_ptr<Signature> signature);
    Macro(Lisple::uptr_sig_v signatures);

    std::string to_string(int depth = -1) const override;
  };

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                Object& arg_array,
                                                sptr_sobject_v& body);

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                Object& arg_array,
                                                ptr_exec_node_v& body);

  std::shared_ptr<DetachedFunction> create_detached_function(Context& ctx,
                                                             Object& arg_array,
                                                             sptr_sobject_v& body);

  std::shared_ptr<DetachedFunction> create_detached_function(Context& ctx,
                                                             Object& arg_array,
                                                             ptr_exec_node_v& body);

} // namespace Lisple

#endif
