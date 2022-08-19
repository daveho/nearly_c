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

#include <memory>
#include <cassert>
#include "exceptions.h"
#include "node.h"
#include "parse.tab.h"
#include "lex.yy.h"
#include "parser_state.h"
#include "context.h"

Context::Context()
  : m_ast(nullptr) {
}

Context::~Context() {
  delete m_ast;
}

struct CloseFile {
  void operator()(FILE *in) {
    if (in != nullptr) {
      fclose(in);
    }
  }
};

void Context::parse(const std::string &filename) {
  // open the input source file
  std::unique_ptr<FILE, CloseFile> in(fopen(filename.c_str(), "r"));
  if (!in) {
    RuntimeError::raise("Couldn't open '%s'", filename.c_str());
  }

  // create an initialize ParserState; note that its destructor
  // will take responsibility for cleaning up the lexer state
  std::unique_ptr<ParserState> pp(new ParserState);
  pp->cur_loc = Location(filename, 1, 1);

  // prepare the lexer
  yylex_init(&pp->scan_info);
  yyset_in(in.get(), pp->scan_info);

  // make the ParserState available from the lexer state
  yyset_extra(pp.get(), pp->scan_info);

  // parse the input source code
  yyparse(pp.get());

  m_ast = pp->parse_tree;
}