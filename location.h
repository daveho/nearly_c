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
