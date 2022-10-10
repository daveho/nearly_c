// Copyright (c) 2021-2022, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#include <cstdlib>
#include "context.h"
#include "ast.h"
#include "grammar_symbols.h"
#include "node.h"
#include "exceptions.h"

void usage() {
  fprintf(stderr, "Usage: nearly_c [options...] <filename>\n"
                  "Options:\n"
                  "  -l   print tokens\n"
                  "  -p   print parse tree\n");
  exit(1);
}

enum class Mode {
  PRINT_TOKENS,
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
    if (arg == "-l") {
      mode = Mode::PRINT_TOKENS;
    } else if (arg == "-p") {
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
  } catch (BaseException &ex) {
    const Location &loc = ex.get_loc();
    if (loc.is_valid()) {
      fprintf(stderr, "%d:%d:Error: %s\n", loc.get_line(), loc.get_col(), ex.what());
    } else {
      fprintf(stderr, "Error: %s\n", ex.what());
    }
    exit(1);
  }

  return 0;
}

void process_source_file(const std::string &filename, Mode mode) {
  Context ctx;

  if (mode == Mode::PRINT_TOKENS) {
    std::vector<Node *> tokens;
    ctx.scan_tokens(filename, tokens);
    for (auto i = tokens.begin(); i != tokens.end(); ++i) {
      Node *tok = *i;
      printf("%d:%s[%s]\n", tok->get_tag(), get_grammar_symbol_name(tok->get_tag()), tok->get_str().c_str());
      delete tok;
    }
  } else {
    // Parse the input
    ctx.parse(filename);

    if (mode == Mode::PRINT_PARSE_TREE) {
      // Note that we use an ASTTreePrint object to print the parse
      // tree. That way, the parser can build either a parse tree or
      // an AST, and tree printing should work correctly.
      Node *ast = ctx.get_ast();
      ASTTreePrint ptp;
      ptp.print(ast);
    } else if (mode == Mode::COMPILE) {
      printf("TODO: compile the source code\n");
    }
  }
}
