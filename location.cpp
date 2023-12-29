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
