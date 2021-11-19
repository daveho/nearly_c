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

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "location.h"

class Node {
private:
  int m_tag;
  std::vector<Node *> m_kids;
  std::string m_strval;
  long m_ival;
  Location m_loc;
  bool m_loc_was_set_explicitly;

  // no value semantics
  Node(const Node &);
  Node &operator=(const Node &);

  Node(int tag, const std::string &strval, long ival, const std::vector<Node *> &kids);
  Node(int tag, const std::string &strval, long ival, const std::initializer_list<Node *> kids);

public:
  typedef std::vector<Node *>::const_iterator const_iterator;

  // Node::I is a concise shorthand for std::initializer_list<Node *>,
  // and is necessary when creating node objects with initializer lists
  // (for adding children) using an Arena
  typedef std::initializer_list<Node *> I;

  Node(int tag);
  Node(int tag, std::initializer_list<Node *> kids);
  Node(int tag, const std::vector<Node *> &kids);

  Node(int tag, const std::string &strval);
  Node(int tag, long ival);
  Node(int tag, const std::string &strval, long ival);

  virtual ~Node();

  int get_tag() const { return m_tag; }
  std::string get_strval() const { return m_strval; }
  long get_ival() const { return m_ival; }

  void set_strval(const std::string &strval) { m_strval = strval; }
  void set_ival(long ival) { m_ival = ival; }

  void append_kid(Node *kid);
  void prepend_kid(Node *kid);
  unsigned get_num_kids() const { return unsigned(m_kids.size()); }
  Node *get_kid(unsigned index) const { return m_kids.at(index); }
  Node *get_last_kid() const { return m_kids.back(); }

  const_iterator cbegin() const { return m_kids.cbegin(); }
  const_iterator cend() const { return m_kids.cend(); }

  void set_loc(const Location &loc) { m_loc = loc; m_loc_was_set_explicitly = true; }
  const Location &get_loc() const { return m_loc; }
};

#endif // NODE_H
