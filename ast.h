#ifndef AST_H
#define AST_H

#include "grammar_symbols.h"
#include "treeprint.h"
class Node;

enum ASTNodeTag {
  AST_UNIT = 10000,
};

class ASTBuilder {
public:
  ASTBuilder();
  ~ASTBuilder();

  Node *build_ast(Node *t);

private:
  Node *flatten(int ast_tag, Node *t);
};

class ASTTreePrint : public ParseTreePrint {
public:
  ASTTreePrint();
  virtual ~ASTTreePrint();

  virtual std::string node_tag_to_string(int tag) const;
};


#endif // AST_H
