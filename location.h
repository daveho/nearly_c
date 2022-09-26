// Copyright (c) 2021, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location {
private:
  std::string m_srcfile;
  int m_line, m_col;

public:
  Location();
  Location(const std::string &srcfile, int line, int col);
  Location(const Location &other);
  ~Location();

  Location &operator=(const Location &rhs);

  bool is_valid() const { return m_line > 0; }

  std::string get_srcfile() const { return m_srcfile; }
  int get_line() const { return m_line; }
  int get_col() const { return m_col; }

  void advance(int num_cols) { m_col += num_cols; }

  void next_line() { m_line++; m_col = 1; }
};

#endif // LOCATION_H
