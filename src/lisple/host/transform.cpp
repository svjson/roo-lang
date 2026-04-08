
#include "lisple/host/transform.h"

namespace Lisple
{

  // -----------------------------------------------------------------------
  // rtval_from: C++ value -> sptr_rtval
  // -----------------------------------------------------------------------

  sptr_rtval rtval_from(int v)
  {
    return RTValue::number(v);
  }
  sptr_rtval rtval_from(long v)
  {
    return RTValue::number(v);
  }
  sptr_rtval rtval_from(float v)
  {
    return RTValue::number(static_cast<double>(v));
  }
  sptr_rtval rtval_from(double v)
  {
    return RTValue::number(v);
  }
  sptr_rtval rtval_from(const std::string& v)
  {
    return RTValue::string(v);
  }
  sptr_rtval rtval_from(bool v)
  {
    return RTValue::boolean(v);
  }
  sptr_rtval rtval_from(char v)
  {
    return RTValue::character(v);
  }

  // -----------------------------------------------------------------------
  // rtval_to<T>: sptr_rtval -> C++ value
  //
  // Primary template is intentionally left undefined - an unsupported T will
  // produce a clear link-time (or explicit-specialisation) error.
  // -----------------------------------------------------------------------

  template <> int rtval_to<int>(const sptr_rtval& v)
  {
    return v->i32();
  }
  template <> long rtval_to<long>(const sptr_rtval& v)
  {
    return v->i64();
  }
  template <> float rtval_to<float>(const sptr_rtval& v)
  {
    return v->f32();
  }
  template <> double rtval_to<double>(const sptr_rtval& v)
  {
    return v->f64();
  }
  template <> std::string rtval_to<std::string>(const sptr_rtval& v)
  {
    return v->str();
  }
  template <> bool rtval_to<bool>(const sptr_rtval& v)
  {
    return std::get<bool>(v->value);
  }
  template <> char rtval_to<char>(const sptr_rtval& v)
  {
    return std::get<char>(v->value);
  }
  template <> unsigned char rtval_to<unsigned char>(const sptr_rtval& v)
  {
    return std::get<char>(v->value);
  }

  // -----------------------------------------------------------------------
  // rtval_matches: cache check - true if the cached RTValue represents the
  // same C++ value as the one supplied.
  // -----------------------------------------------------------------------

  bool rtval_matches(const sptr_rtval& c, int v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_rtval& c, long v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_rtval& c, float v)
  {
    return c && c->is_number(static_cast<double>(v));
  }
  bool rtval_matches(const sptr_rtval& c, double v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_rtval& c, const std::string& v)
  {
    return c && c->is_string(v);
  }

} // namespace Lisple
