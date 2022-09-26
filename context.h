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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
class Node;

// The Context class gathers together all of the objects/data
// used in the compilation process, and orchestrates the various
// passes and transformations.
class Context {
private:
  Node *m_ast;

  // copy ctor and assignment operator not allowed
  Context(const Context &);
  Context &operator=(const Context &);

public:
  Context();
  ~Context();

  // Parse an input file and build an AST
  void parse(const std::string &filename);

  // Get pointer to root of AST
  Node *get_ast() const { return m_ast; }

  // TODO: add member functions for semantic analysis, code generation, etc.
};

#endif // CONTEXT_H
