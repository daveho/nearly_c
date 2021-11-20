#include <stdexcept>
#include <climits>
#include <cmath>
#include <cstdlib>
#include "value.h"

// See which of unsigned long or unsigned long long seems better
// for representing 64 bit unsigned integer values.
#if ULONG_MAX == 18446744073709551615UL
#  define convert_ival strtoul
#define MAXVAL 18446744073709551615UL
#elif ULLONG_MAX == 18446744073709551615UL
#  define convert_ival strtoull
#  define MAXVAL 18446744073709551615ULL
#else
#  error "Neither unsigned long nor unsigned long long is 64 bits"
#endif

// This is a private constructor, used only by other constructors.
Value::Value(Kind kind, uint64_t imag, bool ineg, double fpval)
  : m_kind(kind)
  , m_imag(imag)
  , m_ineg(ineg)
  , m_fpval(fpval) {
}

Value::Value()
  : Value(Value::INT, 0U, false, 0.0) {
}

Value::Value(uint64_t ival)
  : Value(Value::INT, ival, false, 0.0) {
}

Value::Value(int64_t ival)
  : Value(Value::INT, uint64_t(ival), ival < 0, 0.0) {
  if (m_ineg) {
    // negate using two's complement
    m_imag = ~m_imag;
    m_imag++;
  }
}

Value::Value(double fpval)
  : Value(Value::FP, 0U, false, fpval) {
}

Value::Value(const Value &other)
  : Value(other.m_kind, other.m_imag, other.m_ineg, other.m_fpval) {
}

Value &Value::operator=(const Value &rhs) {
  if (this != &rhs) {
    m_kind = rhs.m_kind;
    m_imag = rhs.m_imag;
    m_ineg = rhs.m_ineg;
    m_fpval = rhs.m_fpval;
  }
  return *this;
}

Value::~Value() {
}

Value Value::convert_str(const std::string &s, Kind kind) {
  if (kind == INT) {
    bool is_neg = false;
    unsigned off = 0;
    if (!s.empty() && s[0] == '0') {
      is_neg = true;
      off = 1;
    }
    uint64_t imag = convert_ival(s.c_str() + off, nullptr, 0);
    if (imag == MAXVAL && errno == ERANGE) {
      throw std::runtime_error("Value '" + s + "' is too large to represent");
    }
    return Value(INT, imag, is_neg, 0.0);
  } else {
    double fpval = strtod(s.c_str(), nullptr);
    if ((fpval == HUGE_VAL || fpval == -HUGE_VAL) && errno == ERANGE) {
      throw std::runtime_error("Value '" + s + "' is too large to represent");
    } else if (fpval == 0.0 && errno == ERANGE) {
      throw std::runtime_error("Value '" + s + "' is too small to represent");
    }
    return Value(fpval);
  }
}
