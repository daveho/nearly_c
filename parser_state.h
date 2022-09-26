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

#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include <vector>
#include "location.h"
class Node;

struct ParserState {
  // To avoid depending on yyscan_t, just hard-code knowledge that
  // yyscan_t is just a typedef for void *
  void *scan_info;

  // current location (used by lexer)
  Location cur_loc;

  // Pointer to root of parse tree or AST
  Node *parse_tree;

  // Vector of pointers to Nodes created by the lexer to represent tokens.
  // This can be used to clean up any tokens that aren't incorporated
  // into the tree built by the parser.
  std::vector<Node *> tokens;

  ParserState() : scan_info(nullptr), parse_tree(nullptr) { }
};

#endif // PARSER_STATE_H
