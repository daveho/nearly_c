// Copyright (c) 2021-2022, David H. Hovemeyer <david.hovemeyer@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include "location.h"

// Base type for exceptions indicating syntax or semantic errors
class BaseException : public std::runtime_error {
private:
  Location m_loc;

public:
  BaseException(const Location &loc, const std::string &desc);
  BaseException(const BaseException &other);
  virtual ~BaseException();

  bool has_location() const { return m_loc.is_valid(); }

  const Location &get_loc() const;
};

#ifdef __GNUC__
#  define RT_PRINTF_FORMAT __attribute__ ((format (printf, 1, 2), noreturn))
#  define EX_PRINTF_FORMAT __attribute__ ((format (printf, 2, 3), noreturn))
#else
#  define RT_PRINTF_FORMAT
#  define EX_PRINTF_FORMAT
#endif

// Exception type for general runtime errors that aren't
// associated with source code
class RuntimeError : public BaseException {
public:
  RuntimeError(const std::string &desc);
  RuntimeError(const RuntimeError &other);
  virtual ~RuntimeError();

  static void raise(const char *fmt, ...) RT_PRINTF_FORMAT;
};

// Exception type for lexical or syntax errors
class SyntaxError : public BaseException {
public:
  SyntaxError(const Location &loc, const std::string &desc);
  SyntaxError(const SyntaxError &other);
  virtual ~SyntaxError();

  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

// Exception type for semantic errors
class SemanticError : public BaseException {
public:
  SemanticError(const Location &loc, const std::string &desc);
  SemanticError(const SemanticError &other);
  virtual ~SemanticError();

  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

// Exception type for evaluation errors (reference to undefined
// variable, divide by 0, etc.)
class EvaluationError : public BaseException {
public:
  EvaluationError(const Location &loc, const std::string &desc);
  EvaluationError(const EvaluationError &other);
  virtual ~EvaluationError();

  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

#endif // EXCEPTIONS_H
