#include <cstdio>
#include "node.h"
#include "parse.tab.h"
#include "lex.yy.h"
#include "parser_state.h"
#include "grammar_symbols.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: nearly_c <filename>\n");
    exit(1);
  }

  const char *filename = argv[1];
  FILE *in = fopen(filename, "r");
  if (!in) {
    fprintf(stderr, "Couldn't open '%s'\n", filename);
    exit(1);
  }

  ParserState *pp = new ParserState;
  pp->cur_loc = Location(filename, 1, 1);

  yylex_init(&pp->scan_info);
  yyset_in(in, pp->scan_info);

  // make the ParserState available from the lexer state
  yyset_extra(pp, pp->scan_info);

  pp->parse_tree = nullptr;

  yyparse(pp);

  ParseTreePrint ptp;
  ptp.print(pp->parse_tree);

  delete pp;

  return 0;
}
