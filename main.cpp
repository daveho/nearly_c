#include <cstdio>
#include "node.h"
#include "parse.tab.h"
#include "lex.yy.h"
#include "parser_state.h"
#include "grammar_symbols.h"
#include "ast.h"

void usage() {
  fprintf(stderr, "Usage: nearly_c [options...] <filename>\n"
                  "Options:\n"
                  "  -p   print parse tree\n"
                  "  -a   print AST\n");
  exit(1);
}

const int need_ast        = (1 << 1);
const int need_compile    = (1 << 2);

enum class Mode {
  PRINT_PARSE_TREE = 10000,
  PRINT_AST        = 11000 + (need_ast),
  COMPILE          = 12000 + (need_ast | need_compile),
};

int main(int argc, char **argv) {
  if (argc < 2) {
    usage();
  }

  Mode mode = Mode::COMPILE;

  int index = 1;
  while (index < argc) {
    std::string arg(argv[index]);
    if (arg == "-p") {
      mode = Mode::PRINT_PARSE_TREE;
    } else if (arg == "-a") {
      mode = Mode::PRINT_AST;
    } else {
      break;
    }
    index++;
  }

  if (index >= argc) {
    usage();
  }

  const char *filename = argv[index];
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

  yyparse(pp);

  Node *ast = nullptr;
  if (int(mode) & need_ast) {
    ASTBuilder ast_builder;
    ast = ast_builder.build_ast(pp->parse_tree);
  }

  if (mode == Mode::PRINT_PARSE_TREE) {
    ParseTreePrint ptp;
    ptp.print(pp->parse_tree);
  } else if (mode == Mode::PRINT_AST) {
    ASTTreePrint atp;
    atp.print(ast);
  } else if (mode == Mode::COMPILE) {
    printf("TODO: compile the source code\n");
  }

  delete pp;

  return 0;
}
