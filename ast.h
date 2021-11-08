#ifndef AST_H
#define AST_H

#include "grammar_symbols.h"
#include "treeprint.h"
class Node;

enum ASTNodeTag {
  AST_UNIT = 10000,
  AST_VARIABLE_DECLARATION,
  AST_STRUCT_TYPE,
  AST_UNION_TYPE,
  AST_BASIC_TYPE,
  AST_DECLARATOR_LIST,
  AST_NAMED_DECLARATOR,
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
};

class ASTTreePrint : public ParseTreePrint {
public:
  ASTTreePrint();
  virtual ~ASTTreePrint();

  virtual std::string node_tag_to_string(int tag) const;
};


#endif // AST_H
