// Copyright (c) 2021-2023, David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#include "node.h"

// Private constructor, used only by other constructors
Node::Node(int tag, const std::string &str, const std::vector<Node *> &kids)
  : m_tag(tag)
  , m_kids(kids)
  , m_str(str)
  , m_loc_was_set_explicitly(false) {
}

// Private constructor, used only by other constructors
Node::Node(int tag, const std::string &str, const std::initializer_list<Node *> kids)
  : m_tag(tag)
  , m_kids(kids)
  , m_str(str)
  , m_loc_was_set_explicitly(false) {
}

Node::Node(int tag)
  : Node(tag, "", {}) {
}

Node::Node(int tag, std::initializer_list<Node *> kids)
  : Node(tag, "", kids) {
  // parent node's location defaults to first kid's location
  if (!m_kids.empty()) {
    m_loc = m_kids[0]->get_loc();
  }
}

Node::Node(int tag, const std::vector<Node *> &kids)
  : Node(tag, "", kids) {
  // parent node's location defaults to first kid's location
  if (!m_kids.empty()) {
    m_loc = m_kids[0]->get_loc();
  }
}

Node::Node(int tag, const std::string &str)
  : Node(tag, str, {}) {
}

Node::~Node() {
  // delete child nodes
  for (auto i = m_kids.begin(); i != m_kids.end(); ++i) {
    delete *i;
  }
}

void Node::append_kid(Node *kid) {
  m_kids.push_back(kid);
  // parent node's location defaults to first kid's location
  if (!m_loc.is_valid()) {
    m_loc = kid->get_loc();
  }
}

void Node::prepend_kid(Node *kid) {
  m_kids.insert(m_kids.begin(), kid);

  // Here, we update the parent's location unconditionally
  // (since we generally want the parent's location to match that
  // of the first child), *unless* the parent's location was
  // set explicitly.
  if (kid->get_loc().is_valid() && !m_loc_was_set_explicitly) {
    m_loc = kid->get_loc();
  }
}

void Node::shift_kid() {
  m_kids.erase(m_kids.begin());
  if (!m_kids.empty()) {
    m_loc = m_kids.front()->get_loc();
    m_loc_was_set_explicitly = false;
  }
}
