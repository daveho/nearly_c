#include "location.h"

Location::Location()
  : m_srcfile("<unknown>")
  , m_line(-1)
  , m_col(-1) {
}

Location::Location(const std::string &srcfile, int line, int col)
  : m_srcfile(srcfile)
  , m_line(line)
  , m_col(col) {
}

Location::Location(const Location &other)
  : m_srcfile(other.m_srcfile)
  , m_line(other.m_line)
  , m_col(other.m_col) {
}

Location::~Location() {
}

Location &Location::operator=(const Location &rhs) {
  if (this != &rhs) {
    m_srcfile = rhs.m_srcfile;
    m_line = rhs.m_line;
    m_col = rhs.m_col;
  }
  return *this;
}
