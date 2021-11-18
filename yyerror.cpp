#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "parser_state.h"

void yyerror(struct ParserState *pp, const char *msg, ...) {
  // This function could be reimplemented to throw an exception

  va_list args;
  va_start(args, msg);
  fprintf(stderr, "Error:%d:%d: ", pp->cur_loc.get_line(), pp->cur_loc.get_col());
  vfprintf(stderr, msg, args);
  va_end(args);

  exit(1);
}

