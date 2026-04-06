
#ifndef LISPLE__HOST__TRANSFORM_H
#define LISPLE__HOST__TRANSFORM_H

#include <string>
#include <variant>

#include <lisple/runtime/value.h>

namespace Lisple
{

  // -----------------------------------------------------------------------
  // rtval_from: C++ value → sptr_rtval
  // -----------------------------------------------------------------------

  inline sptr_rtval rtval_from(int v)
  {
    return RTValue::number(v);
  }
  inline sptr_rtval rtval_from(long v)
  {
    return RTValue::number(v);
  }
  inline sptr_rtval rtval_from(float v)
  {
    return RTValue::number(static_cast<double>(v));
  }
  inline sptr_rtval rtval_from(double v)
  {
    return RTValue::number(v);
  }
  inline sptr_rtval rtval_from(const std::string& v)
  {
    return RTValue::string(v);
  }
  inline sptr_rtval rtval_from(bool v)
  {
    return RTValue::boolean(v);
  }
  inline sptr_rtval rtval_from(char v)
  {
    return RTValue::character(v);
  }

  // -----------------------------------------------------------------------
  // rtval_to<T>: sptr_rtval → C++ value
  //
  // Primary template is intentionally left undefined - an unsupported T will
  // produce a clear link-time (or explicit-specialisation) error.
  // -----------------------------------------------------------------------

  template <typename T> T rtval_to(const sptr_rtval& v);

  template <> inline int rtval_to<int>(const sptr_rtval& v)
  {
    return v->i32();
  }
  template <> inline long rtval_to<long>(const sptr_rtval& v)
  {
    return v->i64();
  }
  template <> inline float rtval_to<float>(const sptr_rtval& v)
  {
    return v->f32();
  }
  template <> inline double rtval_to<double>(const sptr_rtval& v)
  {
    return v->f64();
  }
  template <> inline std::string rtval_to<std::string>(const sptr_rtval& v)
  {
    return v->str();
  }
  template <> inline bool rtval_to<bool>(const sptr_rtval& v)
  {
    return std::get<bool>(v->value);
  }
  template <> inline char rtval_to<char>(const sptr_rtval& v)
  {
    return std::get<char>(v->value);
  }

  // -----------------------------------------------------------------------
  // rtval_matches: cache check - true if the cached RTValue represents the
  // same C++ value as the one supplied.
  // -----------------------------------------------------------------------

  inline bool rtval_matches(const sptr_rtval& c, int v)
  {
    return c && c->is_number(v);
  }
  inline bool rtval_matches(const sptr_rtval& c, long v)
  {
    return c && c->is_number(v);
  }
  inline bool rtval_matches(const sptr_rtval& c, float v)
  {
    return c && c->is_number(static_cast<double>(v));
  }
  inline bool rtval_matches(const sptr_rtval& c, double v)
  {
    return c && c->is_number(v);
  }
  inline bool rtval_matches(const sptr_rtval& c, const std::string& v)
  {
    return c && c->is_string(v);
  }

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
