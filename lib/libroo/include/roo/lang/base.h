
#ifndef ROO__LANG__BASE_H
#define ROO__LANG__BASE_H

#include <roo/exec.h>

namespace Roo
{

  /*!
   * @brief Defines a var in the current namespace, and optionally
   * assigns it a value.
   *
   * Usage:
   * @code
   * (def x 5)
   * => 5
   *
   * (def y)
   * => nil
   *
   * (def my-map {:a 1 :b 2})
   * => {:a 1 :b 2}
   * @endcode
   */
  SPECIAL_FORM_DECL(DefForm, def, def_docstring)

  /*!
   * @brief Evaluates the body forms sequentially and returns the
   * value of the last form.
   *
   * Usage:
   * @code
   * (do (prn "Hello") (prn "World"))
   * => nil
   *
   * (do
   *   (some-side-effect! 10)
   *   (other-side-effect! 20)
   *   :status/success)
   * => :status/success
   * @endcode
   */
  SPECIAL_FORM_DECL(DoForm, do)

  /*!
   * @brief Evaluate a Roo form or a string containing Roo code.
   *
   * Usage:
   * @code
   * (eval '(+ 1 1)) => 2
   * (eval "(+ 1 1)") => 2
   * @endcode
   */
  FUNC(EvalFunction, eval_string, eval_form)

  /*!
   * @brief Evaluate a file, loading all forms it contains.
   *
   * Usage:
   * @code
   * (include "my-file.lsp")
   * @endcode
   */
  FUNC(IncludeFunction, include)

  /*!
   * @brief Tests if all arguments are truthy
   *
   * Returns true if all arguments evaluate as true/truthy, or false if any
   * argument evaluates as false/falsy.
   *
   * Arguments are lazily evaluated, and evaluation stops if a falsy value
   * is encountered.
   *
   * Usage:
   * @code
   * (and true [] "string")
   * => true
   *
   * (and true false nil)
   * => false
   * @endcode
   */
  SPECIAL_FORM_DECL(AndForm, and)

  /*!
   * @brief Create a keyword from the value of a string or symbol.
   *
   * Usage:
   * @code
   * (keyword "a string") => :a-string
   * (keyword 'a-symbol) => :a-symbol
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | The string or symbol to convert to a keyword.                      |
   */
  FUNC(KeywordFunction, keyword);

  /*!
   * @brief Query if a value is a keyword.
   *
   * Usage:
   * @code
   * (keyword? "a string") => false
   * (keyword? :thing) => true
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | The value whose type to query.                                     |
   */
  FUNC(KeywordPFunction, keyword);

  /*!
   * @brief Extracts the name part of a qualified identifier as a string.
   *
   * Usage:
   * @code
   * (name :accept/ok) => "ok"
   * (name :ok) => "ok"
   * @endcode
   */
  FUNC(NameFunction, name)

  /*!
   * @brief Declare the namespace of the current source file.
   *
   * Usage:
   * @code
   * (ns sample.core)
   * (ns sample.core (:require [roo.string :as string]))
   * @endcode
   */
  SPECIAL_FORM_DECL(NsForm, ns);

  /*!
   * @brief Tests if any arguments are truthy and returns the first truthy
   * argument. If all arguments are falsy, returns the last evaluated argument.
   *
   * Arguments are lazily evaluated, and evaluation stops if a truthy value
   * is encountered. With no arguments, returns nil.
   *
   * Usage:
   * @code
   * (or {:x 1 :y 12} false)
   * => {:x 1 :y 12}
   *
   * (or nil nil)
   * => nil
   *
   * (or nil false)
   * => false
   *
   * (or false nil)
   * => nil
   *
   * (or false false)
   * => false
   *
   * (or nil 2)
   * => 2
   * @endcode
   */
  SPECIAL_FORM_DECL(OrForm, or)

  /*!
   * @brief Print arguments to stdout, separated by spaces, followed by a newline.
   *
   * Usage:
   * @code
   * (prn "Hello" "World")
   * @endcode
   */
  FUNC(PrnFunction, prn)

  /*!
   * @brief Resolve the value of a symbol programmatically.
   *
   * Usage:
   * @code
   * (resolve '+) => <fn>
   * @endcode
   */
  FUNC(ResolveFunction, resolve)

  /*!
   * @brief Reseed the runtime-local random generator.
   *
   * Usage:
   * @code
   * (random-seed! 1234)
   * @endcode
   */
  FUNC(RandomSeedBangFunction, random_seed_bang)

  /*!
   * @brief Return a random integer in the range from zero to the given upper bound.
   *
   * Usage:
   * @code
   * (rnd 10)
   * => 4
   * @endcode
   *
   * | Arg         | Description                                                        |
   * | ----------- | ------------------------------------------------------------------ |
   * | upper-bound | Exclusive upper bound for the generated integer.                   |
   */
  FUNC(RndFunction, rnd)

  /*!
   * @brief Evaluate body forms with a temporary deterministic random seed.
   *
   * Usage:
   * @code
   * (with-random-seed 1234
   *   (rnd 10))
   * @endcode
   */
  SPECIAL_FORM_DECL(WithRandomSeedForm, with_random_seed)

  /*!
   * @brief Set a new value for a symbol in the lexical scope, namespace or
   * nested structure.
   */
  SPECIAL_FORM_DECL(SetBangForm, set)

  /*!
   * @brief Extracts the qualifier/namespace part of a qualified identifier as
   * a string.
   *
   * Usage:
   * @code
   * (qualifier :accept/ok) => "accept"
   * (qualifier :ok) => nil
   * @endcode
   */
  FUNC(QualifierFunction, qualifier)

  /*!
   * @brief Query if a value is a string.
   *
   * Usage:
   * @code
   * (keyword? "a string") => true
   * (keyword? :thing) => false
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | The value whose type to query.                                     |
   */
  FUNC(StringPFunction, stringp);

  /*!
   * @brief Returns a string representation of the runtime type of the argument.
   *
   * Usage:
   * @code
   * (type-of 123) => "number"
   * (type-of true) => "boolean"
   * (type-of nil) => "nil"
   * @endcode
   */
  FUNC(TypeOfFunction, type_of)

} // namespace Roo

#endif /* ROO__LANG__BASE_H */
