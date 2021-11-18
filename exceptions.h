#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include "location.h"

// Base type for exceptions indicating syntax or semantic errors
class NearlyCException : public std::runtime_error {
private:
  Location m_loc;

public:
  NearlyCException(const Location &loc, const std::string &desc);
  NearlyCException(const NearlyCException &other);
  virtual ~NearlyCException();

  const Location &get_loc() const;
};

#ifdef __GNUC__
#  define RT_PRINTF_FORMAT __attribute__ ((format (printf, 1, 2)))
#  define EX_PRINTF_FORMAT __attribute__ ((format (printf, 2, 3)))
#else
#  define RT_PRINTF_FORMAT
#  define EX_PRINTF_FORMAT
#endif

// Exception type for general runtime errors that aren't
// associated with source code
class RuntimeError : public NearlyCException {
public:
  RuntimeError(const std::string &desc);
  RuntimeError(const RuntimeError &other);
  virtual ~RuntimeError();

  static void raise(const char *fmt, ...) RT_PRINTF_FORMAT;
};

// Exception type for syntax errors
class SyntaxError : public NearlyCException {
public:
  SyntaxError(const Location &loc, const std::string &desc);
  SyntaxError(const SyntaxError &other);
  virtual ~SyntaxError();

  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

// Exception type for semantic errors
class SemanticError : public NearlyCException {
public:
  SemanticError(const Location &loc, const std::string &desc);
  SemanticError(const SemanticError &other);
  virtual ~SemanticError();

  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

#endif // EXCEPTIONS_H
