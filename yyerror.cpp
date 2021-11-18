#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "parser_state.h"
#include "cpputil.h"
#include "exceptions.h"

void yyerror(struct ParserState *pp, const char *msg, ...) {
  // This function could be reimplemented to throw an exception

  va_list args;
  va_start(args, msg);
  std::string errmsg;
  errmsg += cpputil::format("Error:%d:%d: ", pp->cur_loc.get_line(), pp->cur_loc.get_col());
  errmsg += cpputil::vformat(msg, args);
  va_end(args);

  throw SyntaxError(pp->cur_loc, errmsg);
}
