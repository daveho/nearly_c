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

#include <cassert>
#include "node.h"
#include "ast.h"

ASTTreePrint::ASTTreePrint() {
}

ASTTreePrint::~ASTTreePrint() {
}

std::string ASTTreePrint::node_tag_to_string(int tag) const {
  switch (tag) {
  case AST_UNIT: return "AST_UNIT";
  case AST_VARIABLE_DECLARATION: return "AST_VARIABLE_DECLARATION";
  case AST_STRUCT_TYPE: return "AST_STRUCT_TYPE";
  case AST_UNION_TYPE: return "AST_UNION_TYPE";
  case AST_BASIC_TYPE: return "AST_BASIC_TYPE";
  case AST_DECLARATOR_LIST: return "AST_DECLARATOR_LIST";
  case AST_NAMED_DECLARATOR: return "AST_NAMED_DECLARATOR";
  case AST_POINTER_DECLARATOR: return "AST_POINTER_DECLARATOR";
  case AST_ARRAY_DECLARATOR: return "AST_ARRAY_DECLARATOR";
  case AST_FUNCTION_DEFINITION: return "AST_FUNCTION_DEFINITION";
  case AST_FUNCTION_DECLARATION: return "AST_FUNCTION_DECLARATION";
  case AST_FUNCTION_PARAMETER_LIST: return "AST_FUNCTION_PARAMETER_LIST";
  case AST_FUNCTION_PARAMETER: return "AST_FUNCTION_PARAMETER";
  case AST_STATEMENT_LIST: return "AST_STATEMENT_LIST";
  case AST_EMPTY_STATEMENT: return "AST_EMPTY_STATEMENT";
  case AST_EXPRESSION_STATEMENT: return "AST_EXPRESSION_STATEMENT";
  case AST_RETURN_STATEMENT: return "AST_RETURN_STATEMENT";
  case AST_RETURN_EXPRESSION_STATEMENT: return "AST_RETURN_EXPRESSION_STATEMENT";
  case AST_WHILE_STATEMENT: return "AST_WHILE_STATEMENT";
  case AST_DO_WHILE_STATEMENT: return "AST_DO_WHILE_STATEMENT";
  case AST_FOR_STATEMENT: return "AST_FOR_STATEMENT";
  case AST_IF_STATEMENT: return "AST_IF_STATEMENT";
  case AST_IF_ELSE_STATEMENT: return "AST_IF_ELSE_STATEMENT";
  case AST_STRUCT_TYPE_DEFINITION: return "AST_STRUCT_TYPE_DEFINITION";
  case AST_UNION_TYPE_DEFINITION: return "AST_UNION_TYPE_DEFINITION";
  case AST_FIELD_DEFINITION_LIST: return "AST_FIELD_DEFINITION_LIST";
  case AST_BINARY_EXPRESSION: return "AST_BINARY_EXPRESSION";
  case AST_UNARY_EXPRESSION: return "AST_UNARY_EXPRESSION";
  case AST_POSTFIX_EXPRESSION: return "AST_POSTFIX_EXPRESSION";
  case AST_CONDITIONAL_EXPRESSION: return "AST_CONDITIONAL_EXPRESSION";
  case AST_CAST_EXPRESSION: return "AST_CAST_EXPRESSION";
  case AST_FUNCTION_CALL_EXPRESSION: return "AST_FUNCTION_CALL_EXPRESSION";
  case AST_FIELD_REFERENCE_EXPRESSION: return "AST_FIELD_REFERENCE_EXPRESSION";
  case AST_ARGUMENT_EXPRESSION_LIST: return "AST_ARGUMENT_EXPRESSION_LIST";
  case AST_LITERAL_VALUE: return "AST_LITERAL_VALUE";
  case AST_VARIABLE_REFERENCE: return "AST_VARIABLE_REFERENCE";

    // If the tag doesn't match any of the AST node tags,
    // assume it's a parse tree node
  default: return ParseTreePrint::node_tag_to_string(tag);
  }
}
