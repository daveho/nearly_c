#ifndef DRIVER_H
#define DRIVER_H

#include "location.h"
class Scanner;

// The Driver object contains all of the state needed by the lexer
// and parser.
class Driver {
private:
  Scanner *m_scanner;
  Location m_cur_loc;

public:
  Driver();
  ~Driver();

  void set_scanner(Scanner *scanner);
  void set_cur_loc(const Location &loc);

  Scanner *get_scanner() { return m_scanner; }
  Location &get_cur_loc() { return m_cur_loc; }
};

#endif // DRIVER_H
