#include "node.h"
#include "ast.h"

ASTBuilder::ASTBuilder() {
}

ASTBuilder::~ASTBuilder() {
}

Node *ASTBuilder::build_ast(Node *t) {
  switch (t->get_tag()) {
  case NODE_unit:
    // TODO: flatten
    return t;
  default:
    return t;
  }
}

ASTTreePrint::ASTTreePrint() {
}

ASTTreePrint::~ASTTreePrint() {
}

std::string ASTTreePrint::node_tag_to_string(int tag) const {
  switch (tag) {
  case AST_UNIT: return "AST_UNIT";
  default: return "<unknown>";
  }
}
