#! /usr/bin/env ruby

# Copyright (c) 2021-2023, David H. Hovemeyer <david.hovemeyer@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR

# This script reads ast.h and generates:
#
#   ast_visitor.h
#   ast_visitor.cpp
#   ast.cpp

def visit_function_name(tag)
  raise "huh?" if !tag.start_with?('AST_')
  tag = tag[4..-1]
  return "visit_#{tag.downcase}"
end

def gen_ast_visitor_h(outf, ast_tags)

  outf.print <<"EOF1"
#ifndef AST_VISITOR_H
#define AST_VISITOR_H

class Node;

//! Base class for AST visitors.
class ASTVisitor {
public:
  ASTVisitor();
  virtual ~ASTVisitor();

  //! Visit given AST Node.
  //! This will result in a call to an appropriate visitation member
  //! function depending on the Node object's tag value.
  //! For example, if the tag value is `AST_STATEMENT_LIST`, the
  //! `visit_statement_list` member function will be called.
  //!
  //! @param n the Node to visit
  virtual void visit(Node *n);
EOF1

  ast_tags.each do |tag|
    outf.puts <<"EOF_VISIT_FN"
  //! Visit a Node with the `#{tag}` tag value.
  //! @param n a Node with the `#{tag}` tag value
EOF_VISIT_FN
    outf.puts "  virtual void #{visit_function_name(tag)}(Node *n);"
  end

  outf.print <<"EOF2"
  //! Call `visit` on each child Node of the given parent Node.
  //! @param n the parent Node whose children should be visited
  virtual void visit_children(Node *n);

  //! This method is called if the Node being visited is a token
  //! (terminal symbol).
  //! @param n the token (terminal symbol) Node
  virtual void visit_token(Node *n);
};

#endif // AST_VISITOR_H
EOF2
end

def gen_ast_visitor_cpp(outf, ast_tags)
  outf.print <<"EOF3"
#include "node.h"
#include "exceptions.h"
#include "ast.h"
#include "ast_visitor.h"

ASTVisitor::ASTVisitor() {
}

ASTVisitor::~ASTVisitor() {
}
EOF3

  ast_tags.each do |tag|
    outf.print <<"EOF4"

void ASTVisitor::#{visit_function_name(tag)}(Node *n) {
  visit_children(n);
}
EOF4
  end

  outf.print <<"EOF5"

void ASTVisitor::visit(Node *n) {
  // assume that any node with a tag value less than 1000
  // is a token
  if (n->get_tag() < 1000) {
    visit_token(n);
    return;
  }

  switch (n->get_tag()) {
EOF5

  ast_tags.each do |tag|
    outf.puts "  case #{tag}:"
    outf.puts "    #{visit_function_name(tag)}(n); break;"
  end

  outf.print <<"EOF6"
  default:
    RuntimeError::raise("Unknown AST node tag %d", n->get_tag());
  }
}

void ASTVisitor::visit_children(Node *n) {
  for (auto i = n->cbegin(); i != n->cend(); ++i) {
    visit(*i);
  }
}

void ASTVisitor::visit_token(Node *n) {
  // default implementation does nothing
}
EOF6
end

def gen_ast_cpp(outf, ast_tags)
  outf.print <<"EOF7"
#include <cassert>
#include "node.h"
#include "ast.h"

ASTTreePrint::ASTTreePrint() {
}

ASTTreePrint::~ASTTreePrint() {
}

std::string ASTTreePrint::node_tag_to_string(int tag) const {
  switch (tag) {
EOF7

  ast_tags.each do |tag|
    outf.puts "  case #{tag}: return \"#{tag}\";"
  end

  outf.print <<"EOF8"
    // If the tag doesn't match any of the AST node tags,
    // assume it's a parse tree node
  default: return ParseTreePrint::node_tag_to_string(tag);
  }
}
EOF8
end

ast_tags = []

STDIN.each_line do |line|
  line.rstrip!

  if m = /^\s+(AST_\S+)\s*(=\s*\d+\s*)?,(\s*\/\/.*)?$/.match(line)
    ast_tags.push(m[1])
  end
end

#puts "found #{ast_tags.length} AST tags"

File.open('ast_visitor.h', 'w') do |outf|
  gen_ast_visitor_h(outf, ast_tags);
end

File.open('ast_visitor.cpp', 'w') do |outf|
  gen_ast_visitor_cpp(outf, ast_tags)
end

File.open('ast.cpp', 'w') do |outf|
  gen_ast_cpp(outf, ast_tags)
end
