
#ifndef LISPLE__HOST__TRANSFORM_H
#define LISPLE__HOST__TRANSFORM_H

#include <string>
#include <variant>

#include <lisple/runtime/value.h>

namespace Lisple
{

  // -----------------------------------------------------------------------
  // rtval_from: C++ value -> sptr_val
  // -----------------------------------------------------------------------

  sptr_val rtval_from(int v);
  sptr_val rtval_from(short v);
  sptr_val rtval_from(unsigned int v);
  sptr_val rtval_from(unsigned short v);
  sptr_val rtval_from(long v);
  sptr_val rtval_from(float v);
  sptr_val rtval_from(double v);
  sptr_val rtval_from(const std::string& v);
  sptr_val rtval_from(bool v);
  sptr_val rtval_from(char v);
  sptr_val rtval_from(signed char v);
  sptr_val rtval_from(unsigned char v);

  // -----------------------------------------------------------------------
  // rtval_to<T>: sptr_val -> C++ value
  //
  // Primary template is intentionally left undefined - an unsupported T will
  // produce a clear link-time (or explicit-specialisation) error.
  // -----------------------------------------------------------------------

  template <typename T> T rtval_to(const sptr_val& v);

  // -----------------------------------------------------------------------
  // rtval_matches: cache check - true if the cached Value represents the
  // same C++ value as the one supplied.
  // -----------------------------------------------------------------------

  bool rtval_matches(const sptr_val& c, int v);
  bool rtval_matches(const sptr_val& c, short v);
  bool rtval_matches(const sptr_val& c, unsigned int v);
  bool rtval_matches(const sptr_val& c, unsigned short v);
  bool rtval_matches(const sptr_val& c, signed char v);
  bool rtval_matches(const sptr_val& c, unsigned char v);
  bool rtval_matches(const sptr_val& c, long v);
  bool rtval_matches(const sptr_val& c, float v);
  bool rtval_matches(const sptr_val& c, double v);
  bool rtval_matches(const sptr_val& c, const std::string& v);

  // -----------------------------------------------------------------------
  // invoke_setter: calls a member-function setter, deducing the parameter
  // type from the method pointer so no explicit type annotation is needed.
  // -----------------------------------------------------------------------

  template <typename Obj, typename Arg>
  void invoke_setter(Obj& obj, void (Obj::*setter)(Arg), const sptr_val& value)
  {
    (obj.*setter)(rtval_to<std::decay_t<Arg>>(value));
  }

} // namespace Lisple

#endif /* LISPLE__HOST__TRANSFORM_H */
