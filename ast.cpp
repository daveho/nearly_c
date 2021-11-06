#include "node.h"
#include "ast.h"

ASTBuilder::ASTBuilder() {
}

ASTBuilder::~ASTBuilder() {
}

Node *ASTBuilder::build_ast(Node *t) {
  switch (t->get_tag()) {
  case NODE_unit:
    return flatten(AST_UNIT, t);;
  default:
    return t;
  }
}

// Flatten a list. Assume that the list will grow to the right,
// and that the list will be terminated by a node with only
// one child.
Node *ASTBuilder::flatten(int ast_tag, Node *t) {
  std::vector<Node *> ast_kids;

  while (t != nullptr) {
    ast_kids.push_back(build_ast(t->get_kid(0)));
    t = (t->get_num_kids() > 1) ? t->get_last_kid() : nullptr;
  }

  return new Node(ast_tag, ast_kids);
}

ASTTreePrint::ASTTreePrint() {
}

ASTTreePrint::~ASTTreePrint() {
}

std::string ASTTreePrint::node_tag_to_string(int tag) const {
  switch (tag) {
  case AST_UNIT: return "AST_UNIT";

    // If the tag doesn't match any of the AST node tags,
    // assume it's a parse tree node
  default: return ParseTreePrint::node_tag_to_string(tag);
  }
}
