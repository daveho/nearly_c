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

#include <memory>
#include <cstdio>
#include <cstdlib>
#include "node.h"
#include "parse.tab.h"
#include "lex.yy.h"
#include "parser_state.h"
#include "grammar_symbols.h"
#include "ast.h"
#include "exceptions.h"

void usage() {
  fprintf(stderr, "Usage: nearly_c [options...] <filename>\n"
                  "Options:\n"
                  "  -p   print parse tree\n");
  exit(1);
}

enum class Mode {
  PRINT_PARSE_TREE,
  COMPILE,
};

void process_source_file(const std::string &filename, Mode mode);

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
    } else {
      break;
    }
    index++;
  }

  if (index >= argc) {
    usage();
  }

  const char *filename = argv[index];
  try {
    process_source_file(filename, mode);
  } catch (NearlyCException &ex) {
    fprintf(stderr, "%s\n", ex.what());
    exit(1);
  }

  return 0;
}

struct CloseFile {
  void operator()(FILE *in) {
    if (in != nullptr) {
      fclose(in);
    }
  }
};

void process_source_file(const std::string &filename, Mode mode) {
  std::unique_ptr<FILE, CloseFile> in(fopen(filename.c_str(), "r"));
  if (!in) {
    RuntimeError::raise("Error: Couldn't open '%s'", filename.c_str());
  }

  std::unique_ptr<ParserState> pp(new ParserState);
  pp->cur_loc = Location(filename, 1, 1);

  yylex_init(&pp->scan_info);
  yyset_in(in.get(), pp->scan_info);

  // make the ParserState available from the lexer state
  yyset_extra(pp.get(), pp->scan_info);

  yyparse(pp.get());

  if (mode == Mode::PRINT_PARSE_TREE) {
    // Note that we use an ASTTreePrint object to print the parse
    // tree. That way, the parser can build either a parse tree or
    // an AST, and tree printing should work correctly.
    ASTTreePrint ptp;
    ptp.print(pp->parse_tree);
  } else if (mode == Mode::COMPILE) {
    printf("TODO: compile the source code\n");
  }
}
