
#include "lisple/host/transform.h"

namespace Lisple
{

  // -----------------------------------------------------------------------
  // rtval_from: C++ value -> sptr_val
  // -----------------------------------------------------------------------

  sptr_val rtval_from(int v)
  {
    return Value::number(v);
  }
  sptr_val rtval_from(short v)
  {
    return Value::number(static_cast<int>(v));
  }
  sptr_val rtval_from(unsigned int v)
  {
    return Value::number(static_cast<int>(v));
  }
  sptr_val rtval_from(unsigned short v)
  {
    return Value::number(static_cast<int>(v));
  }
  sptr_val rtval_from(long v)
  {
    return Value::number(v);
  }
  sptr_val rtval_from(float v)
  {
    return Value::number(static_cast<double>(v));
  }
  sptr_val rtval_from(double v)
  {
    return Value::number(v);
  }
  sptr_val rtval_from(const std::string& v)
  {
    return Value::string(v);
  }
  sptr_val rtval_from(bool v)
  {
    return Value::boolean(v);
  }
  sptr_val rtval_from(char v)
  {
    return Value::character(v);
  }
  sptr_val rtval_from(signed char v)
  {
    return Value::number(static_cast<int>(v));
  }
  sptr_val rtval_from(unsigned char v)
  {
    return Value::number(static_cast<int>(v));
  }

  // -----------------------------------------------------------------------
  // rtval_to<T>: sptr_val -> C++ value
  //
  // Primary template is intentionally left undefined - an unsupported T will
  // produce a clear link-time (or explicit-specialisation) error.
  // -----------------------------------------------------------------------

  template <> int rtval_to<int>(const sptr_val& v)
  {
    return v->i32();
  }
  template <> short rtval_to<short>(const sptr_val& v)
  {
    return static_cast<short>(v->i32());
  }
  template <> unsigned int rtval_to<unsigned int>(const sptr_val& v)
  {
    return static_cast<unsigned int>(v->i32());
  }
  template <> unsigned short rtval_to<unsigned short>(const sptr_val& v)
  {
    return static_cast<unsigned short>(v->i32());
  }
  template <> long rtval_to<long>(const sptr_val& v)
  {
    return v->i64();
  }
  template <> float rtval_to<float>(const sptr_val& v)
  {
    return v->f32();
  }
  template <> double rtval_to<double>(const sptr_val& v)
  {
    return v->f64();
  }
  template <> std::string rtval_to<std::string>(const sptr_val& v)
  {
    return v->str();
  }
  template <> bool rtval_to<bool>(const sptr_val& v)
  {
    return std::get<bool>(v->value);
  }
  template <> char rtval_to<char>(const sptr_val& v)
  {
    return std::get<char>(v->value);
  }
  template <> signed char rtval_to<signed char>(const sptr_val& v)
  {
    return static_cast<signed char>(v->i32());
  }
  template <> unsigned char rtval_to<unsigned char>(const sptr_val& v)
  {
    return v->ui8();
  }

  // -----------------------------------------------------------------------
  // rtval_matches: cache check - true if the cached Value represents the
  // same C++ value as the one supplied.
  // -----------------------------------------------------------------------

  bool rtval_matches(const sptr_val& c, int v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_val& c, short v)
  {
    return c && c->is_number(static_cast<int>(v));
  }
  bool rtval_matches(const sptr_val& c, unsigned int v)
  {
    return c && c->is_number(static_cast<int>(v));
  }
  bool rtval_matches(const sptr_val& c, unsigned short v)
  {
    return c && c->is_number(static_cast<int>(v));
  }
  bool rtval_matches(const sptr_val& c, signed char v)
  {
    return c && c->is_number(static_cast<int>(v));
  }
  bool rtval_matches(const sptr_val& c, unsigned char v)
  {
    return c && c->is_number(static_cast<int>(v));
  }
  bool rtval_matches(const sptr_val& c, long v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_val& c, float v)
  {
    return c && c->is_number(static_cast<double>(v));
  }
  bool rtval_matches(const sptr_val& c, double v)
  {
    return c && c->is_number(v);
  }
  bool rtval_matches(const sptr_val& c, const std::string& v)
  {
    return c && c->is_string(v);
  }

} // namespace Lisple
