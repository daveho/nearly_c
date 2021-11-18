#include <cstdarg>
#include "parser_state.h"
#include "cpputil.h"
#include "exceptions.h"

void yyerror(struct ParserState *pp, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  std::string errmsg = cpputil::vformat(msg, args);
  va_end(args);

  throw SyntaxError(pp->cur_loc, errmsg);
}

