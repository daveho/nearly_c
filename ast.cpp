#include <cassert>
#include "node.h"
#include "ast.h"

namespace {

bool is_storage_tag(int tag) {
  return tag == NODE_TOK_EXTERN || tag == NODE_TOK_STATIC;
}

}

ASTBuilder::ASTBuilder() {
}

ASTBuilder::~ASTBuilder() {
}

Node *ASTBuilder::build_ast(Node *t) {
  int tag = t->get_tag();

  switch (tag) {
  case NODE_unit:
    return flatten(AST_UNIT, t);;

  case NODE_top_level_declaration:
    {
      int first_tag = t->get_kid(0)->get_tag();
      bool first_is_storage_class = is_storage_tag(first_tag);
      Node *child_ast = build_ast(t->get_kid(first_is_storage_class ? 1 : 0));
      if (first_is_storage_class) {
        child_ast->prepend_kid(t->get_kid(0));
      }
      return child_ast;
    }

  case NODE_simple_variable_declaration:
    return new Node(AST_VARIABLE_DECLARATION,
                    {build_ast(t->get_kid(0)), build_ast(t->get_kid(1))});

  case NODE_basic_type:
    return flatten(AST_BASIC_TYPE, t);

    // Many nonterminal nodes are guaranteed just to have a single
    // child, and all we need to do is build an AST for the child
  case NODE_function_or_variable_declaration_or_definition:
  case NODE_basic_type_keyword:
    return build_ast(t->get_kid(0));

  case NODE_declarator_list:
    return flatten(AST_DECLARATOR_LIST, t);

  case NODE_declarator:
    {
      // for now, the only kind of declarator is just a plain identifier
      assert(t->get_kid(0)->get_tag() == NODE_TOK_IDENT);
      return new Node(AST_NAMED_DECLARATOR, {t->get_kid(0)});
    }

  case NODE_type:
    {
      int first_tag = t->get_kid(0)->get_tag();
      if (first_tag == NODE_TOK_STRUCT) {
        return new Node(AST_STRUCT_TYPE, {t->get_kid(1)});
      } else if (first_tag == NODE_TOK_UNION) {
        return new Node(AST_UNION_TYPE, {t->get_kid(1)});
      } else {
        return build_ast(t->get_kid(0));
      }
    }

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
  case AST_VARIABLE_DECLARATION: return "AST_VARIABLE_DECLARATION";
  case AST_STRUCT_TYPE: return "AST_STRUCT_TYPE";
  case AST_UNION_TYPE: return "AST_UNION_TYPE";
  case AST_BASIC_TYPE: return "AST_BASIC_TYPE";
  case AST_DECLARATOR_LIST: return "AST_DECLARATOR_LIST";
  case AST_NAMED_DECLARATOR: return "AST_NAMED_DECLARATOR";

    // If the tag doesn't match any of the AST node tags,
    // assume it's a parse tree node
  default: return ParseTreePrint::node_tag_to_string(tag);
  }
}
