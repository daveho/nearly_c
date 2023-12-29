// Copyright (c) 2021-2023, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

//! @file
//! Exception types.
//! Note that the preferred way to throw an exception is to invoke
//! the static `raise()` member function in the exception class,
//! since it will support `printf`-style formatting.

//! Base type for exceptions indicating a syntax error,
// a semantic error, or a general runtime error.
class BaseException : public std::runtime_error {
private:
  Location m_loc;

public:
  //! Constructor.
  //! @param loc the source Location of the error
  //! @param desc description of the error
  BaseException(const Location &loc, const std::string &desc);

  //! Copy constructor.
  //! @param other the BaseException object to copy from
  BaseException(const BaseException &other);

  virtual ~BaseException();

  //! Check whether this exception has a valud source Location.
  //! @return true if this exception has a valid source Location,
  //!         false otherwise
  bool has_location() const { return m_loc.is_valid(); }

  //! Get the source Location.
  //! @return the source Location
  const Location &get_loc() const;
};

#ifdef __GNUC__
#  define RT_PRINTF_FORMAT __attribute__ ((format (printf, 1, 2), noreturn))
#  define EX_PRINTF_FORMAT __attribute__ ((format (printf, 2, 3), noreturn))
#else
#  define RT_PRINTF_FORMAT
#  define EX_PRINTF_FORMAT
#endif

//! Exception type for general runtime errors that aren't
//! associated with source code.
class RuntimeError : public BaseException {
public:
  //! Constuctor.
  //! @param desc description of the error
  RuntimeError(const std::string &desc);

  //! Copy constructor.
  //! @param RuntimeError object to copy from
  RuntimeError(const RuntimeError &other);

  virtual ~RuntimeError();

  //! Throw a RuntimeError exception.
  //! The description is generated from `printf`-style formatting.
  //! @param fmt the format string
  //! @param ... argument values (corresponding to conversions in the format string)
  static void raise(const char *fmt, ...) RT_PRINTF_FORMAT;
};

//! Exception type for lexical or syntax errors.
class SyntaxError : public BaseException {
public:
  //! Constructor.
  //! @param loc the source Location
  //! @param desc description of the error
  SyntaxError(const Location &loc, const std::string &desc);

  //! Copy constructor.
  //! @param other the SyntaxError object to copy from
  SyntaxError(const SyntaxError &other);

  virtual ~SyntaxError();

  //! Throw a SyntaxError exception.
  //! The description is generated from `printf`-style formatting.
  //! @param fmt the format string
  //! @param ... argument values (corresponding to conversions in the format string)
  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

//! Exception type for semantic errors.
class SemanticError : public BaseException {
public:
  //! Constructor.
  //! @param loc the source Location
  //! @param desc description of the error
  SemanticError(const Location &loc, const std::string &desc);

  //! Copy constructor.
  //! @param other the SemanticError object to copy from
  SemanticError(const SemanticError &other);

  virtual ~SemanticError();

  //! Throw a SemanticError exception.
  //! The description is generated from `printf`-style formatting.
  //! @param fmt the format string
  //! @param ... argument values (corresponding to conversions in the format string)
  static void raise(const Location &loc, const char *fmt, ...) EX_PRINTF_FORMAT;
};

#endif // EXCEPTIONS_H
