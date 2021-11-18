#include <cstdarg>
#include <cstdio>
#include "cpputil.h"
#include "node.h"
#include "exceptions.h"

////////////////////////////////////////////////////////////////////////
// NearlyCException member functions
////////////////////////////////////////////////////////////////////////

NearlyCException::NearlyCException(const Location &loc, const std::string &desc)
  : std::runtime_error(desc)
  , m_loc(loc) {
}

NearlyCException::NearlyCException(const NearlyCException &other)
  : std::runtime_error(other)
  , m_loc(other.m_loc) {
}

NearlyCException::~NearlyCException() {
}

const Location &NearlyCException::get_loc() const {
  return m_loc;
}

////////////////////////////////////////////////////////////////////////
// RuntimeError member functions
////////////////////////////////////////////////////////////////////////

RuntimeError::RuntimeError(const std::string &desc)
  : NearlyCException(Location(), desc) {
}

RuntimeError::RuntimeError(const RuntimeError &other)
  : NearlyCException(other) {
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
  : NearlyCException(loc, desc) {
}

SyntaxError::SyntaxError(const SyntaxError &other)
  : NearlyCException(other) {
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
  : NearlyCException(loc, desc) {
}

SemanticError::SemanticError(const SemanticError &other)
  : NearlyCException(other) {
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
