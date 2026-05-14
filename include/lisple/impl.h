
#ifndef __LISPLE__IMPL_H_
#define __LISPLE__IMPL_H_

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include <lisple/form.h>
#include <lisple/type.h>

namespace Lisple
{
  extern const std::string EMPTY_STRING;

  /*!
   * @brief Extracts the string value of a lisple symbol, discarding
   * the Lisple syntax it is wrapped in.
   *
   * For example,
   * @code "a string" @endcode yields @code a string @endcode
   * @code :some-key @endcode yields @code some-key @endcode
   *
   * Complex objects are not considered to have a string value, and
   * will return an empty string
   */
  const std::string str_val(const AST::ASTNode& obj) noexcept;

  char char_val(const AST::ASTNode& obj) noexcept;

  /*!
   * @brief Get the unsigned int value of lisple symbol
   */
  unsigned int uint_val(const AST::ASTNode& obj);
  uint8_t uint8_val(const AST::ASTNode& obj);
  int int_val(const AST::ASTNode& obj);
  short short_val(const AST::ASTNode& obj);
  unsigned short ushort_val(const AST::ASTNode& obj);
  float float_val(const AST::ASTNode& obj);

  std::pair<std::string, std::string> split_qualifiable(const std::string& str);

  template <class T> std::shared_ptr<T> sptr_ast_node_cast(sptr_ast_node& obj)
  {
    if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(obj.get()))
    {
      return std::dynamic_pointer_cast<T>(Lisple::to_AST(*wrapper->val));
    }
    else
    {
      return std::dynamic_pointer_cast<T>(obj);
    }
  }

  /*!
   * @brief Create a copy of a sexp form with a substituted child element
   */
  template <class T>
  std::shared_ptr<T> subst_sexp_lmnt(const T& sexp, unsigned int index, sptr_ast_node subst)
  {
    sptr_ast_node_v nchildren = const_cast<T&>(sexp).get_children();
    nchildren.at(index) = subst;
    return std::make_shared<T>(nchildren);
  }

  /*!
   * @brief Prepend the string value of the head element of a list
   */
  std::shared_ptr<AST::List> prepend_list_head(AST::ASTNode& list_obj,
                                               const std::string& prepend_val);

  template <typename T>
  typename std::enable_if<std::is_arithmetic<T>::value, T>::type unwrap_primitive(
    const AST::ASTNode& obj);

  template <typename T>
  typename std::enable_if<!std::is_arithmetic<T>::value, T>::type unwrap_primitive(
    const AST::ASTNode& obj);

  template <typename T>
  sptr_ast_node wrap_primitive(
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type value);

  template <typename T>
  sptr_ast_node wrap_primitive(
    const typename std::enable_if<!std::is_arithmetic<T>::value, T>::type& value);

} // namespace Lisple

#endif
