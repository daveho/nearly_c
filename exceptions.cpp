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

#include <cstdarg>
#include <cstdio>
#include "cpputil.h"
#include "node.h"
#include "exceptions.h"

////////////////////////////////////////////////////////////////////////
// BaseException member functions
////////////////////////////////////////////////////////////////////////

BaseException::BaseException(const Location &loc, const std::string &desc)
  : std::runtime_error(desc)
  , m_loc(loc) {
}

BaseException::BaseException(const BaseException &other)
  : std::runtime_error(other)
  , m_loc(other.m_loc) {
}

BaseException::~BaseException() {
}

const Location &BaseException::get_loc() const {
  return m_loc;
}

////////////////////////////////////////////////////////////////////////
// RuntimeError member functions
////////////////////////////////////////////////////////////////////////

RuntimeError::RuntimeError(const std::string &desc)
  : BaseException(Location(), desc) {
}

RuntimeError::RuntimeError(const RuntimeError &other)
  : BaseException(other) {
}

RuntimeError::~RuntimeError() {
}

void RuntimeError::raise(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  std::string errmsg = cpputil::vformat(fmt, args);
  va_end(args);

  throw RuntimeError(errmsg);
}

////////////////////////////////////////////////////////////////////////
// SyntaxError member functions
////////////////////////////////////////////////////////////////////////

SyntaxError::SyntaxError(const Location &loc, const std::string &desc)
  : BaseException(loc, desc) {
}

SyntaxError::SyntaxError(const SyntaxError &other)
  : BaseException(other) {
}

SyntaxError::~SyntaxError() {
}

void SyntaxError::raise(const Location &loc, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  std::string errmsg = cpputil::vformat(fmt, args);
  va_end(args);

  throw SyntaxError(loc, errmsg);
}

////////////////////////////////////////////////////////////////////////
// SemanticError member functions
////////////////////////////////////////////////////////////////////////

SemanticError::SemanticError(const Location &loc, const std::string &desc)
  : BaseException(loc, desc) {
}

SemanticError::SemanticError(const SemanticError &other)
  : BaseException(other) {
}

SemanticError::~SemanticError() {
}

void SemanticError::raise(const Location &loc, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  std::string errmsg = cpputil::vformat(fmt, args);
  va_end(args);

  throw SemanticError(loc, errmsg);
}

////////////////////////////////////////////////////////////////////////
// EvaluationError member functions
////////////////////////////////////////////////////////////////////////

EvaluationError::EvaluationError(const Location &loc, const std::string &desc)
  : BaseException(loc, desc) {
}

EvaluationError::EvaluationError(const EvaluationError &other)
  : BaseException(other) {
}

EvaluationError::~EvaluationError() {
}

void EvaluationError::raise(const Location &loc, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  std::string errmsg = cpputil::vformat(fmt, args);
  va_end(args);

  throw EvaluationError(loc, errmsg);
}
