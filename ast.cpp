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
  case AST_FUNCTION_DEFINITION: return "AST_FUNCTION_DEFINITION";
  case AST_FUNCTION_DECLARATION: return "AST_FUNCTION_DECLARATION";
  case AST_FUNCTION_PARAMETER_LIST: return "AST_FUNCTION_PARAMETER_LIST";
  case AST_FUNCTION_PARAMETER: return "AST_FUNCTION_PARAMETER";

    // If the tag doesn't match any of the AST node tags,
    // assume it's a parse tree node
  default: return ParseTreePrint::node_tag_to_string(tag);
  }
}
