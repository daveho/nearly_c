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

#include <vector>
#include <utility>
#include <cstdio>
#include <cassert>
#include "node.h"
#include "treeprint.h"

namespace {

// pair of (index, num siblings):
// index is which sibling we're currently printing
typedef std::pair<int, int> StackItem;

struct TreePrintContext {
  std::vector<StackItem> stack;
  const TreePrint *tp_obj;

  TreePrintContext(const TreePrint *tp_obj_)
    : tp_obj(tp_obj_) { }

  void pushctx(int nsibs);
  void popctx();
  void print_node(Node *n);
};

void TreePrintContext::pushctx(int nsibs_) {
  stack.push_back({ 0, nsibs_ });
}

void TreePrintContext::popctx() {
  stack.pop_back();
}

void TreePrintContext::print_node(Node *n) {
  int depth = int(stack.size());
  assert(depth > 0);
  for (int i = 1; i < depth; i++) {
    if (i == depth-1) {
      printf("+--");
    } else {
      int level_index = stack[i].first;
      int level_nsibs = stack[i].second;
      if (level_index < level_nsibs) {
        printf("|  ");
      } else {
        printf("   ");
      }
    }
  }

  int tag = n->get_tag();
  std::string str = n->get_str();

  printf("%s", tp_obj->node_tag_to_string(tag).c_str());
  if (!str.empty()) {
    printf("[%s]", str.c_str());
  }
  printf("\n");
  stack[depth-1].first++;

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
