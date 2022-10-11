// Copyright (c) 2021-2022, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#include "ast.h"
#include "node.h"
#include "cpputil.h"
#include "print_graph.h"

PrintGraph::PrintGraph(Node *root)
  : m_root(root) {
}

PrintGraph::~PrintGraph() {
}

void PrintGraph::print() {
  visit(m_root, "", 0);

  printf("digraph ast {\n");
  printf("  graph [ordering=\"out\"];\n");

  // output ranks so nodes are at the correct heights
  for (std::map<std::string, int>::iterator i = m_node_levels.begin(); i != m_node_levels.end(); i++) {
    std::string rankattr = cpputil::format("{ rank = %d; \"%s\"; }", i->second, i->first.c_str());
    printf("  %s\n", rankattr.c_str());
  }

  // output edges
  for (std::map<std::string, std::vector<std::string>>::iterator i = m_edges.begin(); i != m_edges.end(); i++) {
    for (std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); j++) {
      std::string edge = cpputil::format("\"%s\" -> \"%s\";", i->first.c_str(), j->c_str());
      printf("  %s\n", edge.c_str());
    }
  }

  printf("}\n");
}

int PrintGraph::visit(Node *n, const std::string &parent_name, int level) {
  ASTTreePrint atp; // for getting node tag as string

  int tag = n->get_tag();
  std::map<int, int>::iterator i = m_node_type_count.find(tag);
  int count = (i == m_node_type_count.end()) ? 0 : i->second;
  m_node_type_count[tag] = count + 1;
  std::string tag_name = atp.node_tag_to_string(tag);
  if (tag_name == "TOK_IDENT") {
    tag_name = "identifier";
  }
  std::string node_name = cpputil::format("%s_%d", tag_name.c_str(), count);
  std::string strval = n->get_str();
  if (!strval.empty()) {
    node_name += "\\n[";
    node_name += strval;
    node_name += "]";
  }
  m_node_levels[node_name] = level;
  if (!parent_name.empty()) {
    m_edges[parent_name].push_back(node_name);
  }
  int num_kids = n->get_num_kids();
  int max_level = level;
  for (int j = 0; j < num_kids; j++) {
    int subtree_level = visit(n->get_kid(j), node_name, level + 1);
    if (subtree_level > max_level) {
      max_level = subtree_level;
    }
  }
  return max_level;
}

/*
void ast_print_graph(Node *ast) {
  PrintGraph agp(ast);
  agp.print();
}
*/
