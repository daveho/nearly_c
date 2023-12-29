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

#ifndef LOCATION_H
#define LOCATION_H

#include <string>

//! A Location represents a source code location.
//! Each token and (in theory) each AST node will have a Location.
//! Location objects have value semantics (can be copied and assigned.)
class Location {
private:
  std::string m_srcfile;
  int m_line, m_col;

public:
  //! Default constructor. The Location will be invalid.
  Location();

  //! Location from source file, line number, and column number.
  //! @param srcfile the name of the source file
  //! @param line the line number
  //! @param col the column number
  Location(const std::string &srcfile, int line, int col);

  //! Copy constructor.
  //! @param other the Location object to copy from
  Location(const Location &other);

  ~Location();

  //! Assignment operator.
  //! @param rhs the Location object to copy from
  Location &operator=(const Location &rhs);

  //! Check whether this Location is valid.
  //! @return true if this Location is valid, false if not
  bool is_valid() const { return m_line > 0; }

  //! Get the source file name.
  //! @return the source file name
  std::string get_srcfile() const { return m_srcfile; }

  //! Get the source line number (1 for the first line).
  //! @return the source line number
  int get_line() const { return m_line; }

  //! Get the source column number (1 for the first column).
  //! @return the source column number
  int get_col() const { return m_col; }

  //! Advance the Location by the given number of character positions.
  //! @param num_cols the number of character positions to advance
  void advance(int num_cols) { m_col += num_cols; }

  //! Advance the Location to the beginning of the next line.
  void next_line() { m_line++; m_col = 1; }
};

#endif // LOCATION_H
