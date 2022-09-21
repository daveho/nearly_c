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

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "location.h"
#include "node_base.h"

// Tree node class, suitable for parse trees and ASTs.
// Nodes can also be used as tokens returned by a lexer.
// Note that parent nodes take responsibility for deleting
// their children, so to delete an entire tree, it is
// sufficient to delete the root.

class Node : public NodeBase {
private:
  int m_tag;
  std::vector<Node *> m_kids;
  std::string m_str;
  Location m_loc;
  bool m_loc_was_set_explicitly;

  // no value semantics
  Node(const Node &);
  Node &operator=(const Node &);

  Node(int tag, const std::string &str, const std::vector<Node *> &kids);
  Node(int tag, const std::string &str, const std::initializer_list<Node *> kids);

public:
  typedef std::vector<Node *>::const_iterator const_iterator;

  Node(int tag);
  Node(int tag, std::initializer_list<Node *> kids);
  Node(int tag, const std::vector<Node *> &kids);
  Node(int tag, const std::string &str);

  virtual ~Node();

  int get_tag() const { return m_tag; }
  void set_tag(int tag) { m_tag = tag; }

  std::string get_str() const { return m_str; }
  void set_str(const std::string &str) { m_str = str; }

  void append_kid(Node *kid);
  void prepend_kid(Node *kid);
  unsigned get_num_kids() const { return unsigned(m_kids.size()); }
  Node *get_kid(unsigned index) const { return m_kids.at(index); }
  Node *get_last_kid() const { return m_kids.back(); }

  const_iterator cbegin() const { return m_kids.cbegin(); }
  const_iterator cend() const { return m_kids.cend(); }

  void set_loc(const Location &loc) { m_loc = loc; m_loc_was_set_explicitly = true; }
  const Location &get_loc() const { return m_loc; }

  // do a preorder traversal of the tree, invoking specified
  // function on each node
  template<typename Fn>
  void preorder(Fn fn) {
    fn(this);
    for (auto i = m_kids.begin(); i != m_kids.end(); ++i) {
      (*i)->preorder(fn);
    }
  }

  // invoke a function on each child
  template<typename Fn>
  void each_child(Fn fn) const {
    for (auto i = m_kids.begin(); i != m_kids.end(); ++i) {
      fn(*i);
    }
  }
};

#endif // NODE_H
