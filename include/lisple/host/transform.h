
#ifndef LISPLE__HOST__TRANSFORM_H
#define LISPLE__HOST__TRANSFORM_H

#include <string>
#include <variant>

#include <lisple/runtime/value.h>

namespace Lisple
{

  // -----------------------------------------------------------------------
  // rtval_from: C++ value -> sptr_rtval
  // -----------------------------------------------------------------------

  sptr_rtval rtval_from(int v);
  sptr_rtval rtval_from(unsigned int v);
  sptr_rtval rtval_from(long v);
  sptr_rtval rtval_from(float v);
  sptr_rtval rtval_from(double v);
  sptr_rtval rtval_from(const std::string& v);
  sptr_rtval rtval_from(bool v);
  sptr_rtval rtval_from(char v);

  // -----------------------------------------------------------------------
  // rtval_to<T>: sptr_rtval -> C++ value
  //
  // Primary template is intentionally left undefined - an unsupported T will
  // produce a clear link-time (or explicit-specialisation) error.
  // -----------------------------------------------------------------------

  template <typename T> T rtval_to(const sptr_rtval& v);

  // -----------------------------------------------------------------------
  // rtval_matches: cache check - true if the cached RTValue represents the
  // same C++ value as the one supplied.
  // -----------------------------------------------------------------------

  bool rtval_matches(const sptr_rtval& c, int v);
  bool rtval_matches(const sptr_rtval& c, long v);
  bool rtval_matches(const sptr_rtval& c, float v);
  bool rtval_matches(const sptr_rtval& c, double v);
  bool rtval_matches(const sptr_rtval& c, const std::string& v);

  // -----------------------------------------------------------------------
  // invoke_setter: calls a member-function setter, deducing the parameter
  // type from the method pointer so no explicit type annotation is needed.
  // -----------------------------------------------------------------------

  template <typename Obj, typename Arg>
  void invoke_setter(Obj& obj, void (Obj::*setter)(Arg), const sptr_rtval& value)
  {
    (obj.*setter)(rtval_to<std::decay_t<Arg>>(value));
  }

} // namespace Lisple

#endif /* LISPLE__HOST__TRANSFORM_H */
