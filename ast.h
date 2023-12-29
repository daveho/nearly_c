// Copyright (c) 2021-2023, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#ifndef AST_H
#define AST_H

#include "grammar_symbols.h"
#include "treeprint.h"
class Node;

//! @file
//! ASTNodeTag enumeraion and ASTTreePrint class.

//! Enumeration type providing tag values for AST nodes.
enum ASTNodeTag {
  AST_UNIT = 10000,
  AST_VARIABLE_DECLARATION,
  AST_STRUCT_TYPE,
  AST_UNION_TYPE,
  AST_BASIC_TYPE,
  AST_DECLARATOR_LIST,
  AST_NAMED_DECLARATOR,
  AST_POINTER_DECLARATOR,
  AST_ARRAY_DECLARATOR,
  AST_FUNCTION_DEFINITION,
  AST_FUNCTION_DECLARATION,
  AST_FUNCTION_PARAMETER_LIST,
  AST_FUNCTION_PARAMETER,
  AST_STATEMENT_LIST,
  AST_EMPTY_STATEMENT,
  AST_EXPRESSION_STATEMENT,
  AST_RETURN_STATEMENT,
  AST_RETURN_EXPRESSION_STATEMENT,
  AST_WHILE_STATEMENT,
  AST_DO_WHILE_STATEMENT,
  AST_FOR_STATEMENT,
  AST_IF_STATEMENT,
  AST_IF_ELSE_STATEMENT,
  AST_STRUCT_TYPE_DEFINITION,
  AST_UNION_TYPE_DEFINITION,
  AST_FIELD_DEFINITION_LIST,
  AST_BINARY_EXPRESSION,
  AST_UNARY_EXPRESSION,
  AST_POSTFIX_EXPRESSION,      // like AST_UNARY_EXPRESSION, but for postfix operators
  AST_CONDITIONAL_EXPRESSION,
  AST_CAST_EXPRESSION,
  AST_FUNCTION_CALL_EXPRESSION,
  AST_FIELD_REF_EXPRESSION,
  AST_INDIRECT_FIELD_REF_EXPRESSION,
  AST_ARRAY_ELEMENT_REF_EXPRESSION,
  AST_ARGUMENT_EXPRESSION_LIST,
  AST_VARIABLE_REF,
  AST_LITERAL_VALUE,
  AST_IMPLICIT_CONVERSION, // semantic analysis can add these to mark locations of implicit type conversions
};

//! Support for printing a text representation of an AST.
class ASTTreePrint : public ParseTreePrint {
public:
  ASTTreePrint();
  virtual ~ASTTreePrint();

  virtual std::string node_tag_to_string(int tag) const;
};


#endif // AST_H
