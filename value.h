#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cstdint>

// This class is useful for representing numeric values at
// compile time.  Any integer (signed or unsigned, up to 64 bits)
// or floating point value can be represented.
class Value {
public:
  enum Kind { INT, FP };

private:
  Kind m_kind;       // integer or floating point
  uint64_t m_imag;   // integer magnitude
  bool m_ineg;       // true if value is a negative integer
  double m_fpval;    // floating point value

  Value(Kind kind, uint64_t imag, bool ineg, double fpval);

public:
  Value();
  Value(uint64_t ival);
  Value(int64_t ival);
  Value(double fpval);

  Value(const Value &other);
  Value &operator=(const Value &rhs);

  ~Value();

  bool get_kind() const { return m_kind; }

  // Convert from a string.
  // Throws std::runtime_error if the string can't be converted to
  // a representable value.
  // Any string containing a legal C integer or floating point
  // literal should be converted correctly. (?)
  static Value convert_str(const std::string &s, Kind kind);
};

#endif // VALUE_H
