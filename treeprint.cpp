#include <stdio.h>
#include "node.h"
#include "treeprint.h"

#define MAX_DEPTH 4096

namespace {

struct TreePrintContext {
  int stack_depth;
  int index[MAX_DEPTH];
  int nsibs[MAX_DEPTH];
  const TreePrint *tp_obj;

  TreePrintContext(const TreePrint *tp_obj_)
    : stack_depth(0), tp_obj(tp_obj_) { }

  void pushctx(int nsibs);
  void popctx();
  void print_node(Node *n);
};

void TreePrintContext::pushctx(int nsibs_) {
  int level = stack_depth;
  index[level] = 0;
  nsibs[level] = nsibs_;
  stack_depth++;
}

void TreePrintContext::popctx() {
  stack_depth--;
}

void TreePrintContext::print_node(Node *n) {
  int depth = stack_depth;
  for (int i = 1; i < depth; i++) {
    if (i == depth-1) {
      printf("+--");
    } else {
      int level_index = index[i];
      int level_nsibs = nsibs[i];
      if (level_index < level_nsibs) {
        printf("|  ");
      } else {
        printf("   ");
      }
    }
  }

  int tag = n->get_tag();
  std::string str = n->get_strval();

  printf("%s", tp_obj->node_tag_to_string(tag).c_str());
  if (!str.empty()) {
    printf("[%s]", str.c_str());
  }
  printf("\n");
  index[depth-1]++;

  int nkids = n->get_num_kids();
  pushctx(nkids);
  for (int i = 0; i < nkids; i++) {
    print_node(n->get_kid(i));
  }
  popctx();
}

} // end anonymous namespace

TreePrint::TreePrint() {
}

TreePrint::~TreePrint() {
}

void TreePrint::print(Node *t) const {
  TreePrintContext ctx(this);
  ctx.pushctx(1);
  ctx.print_node(t);
}
