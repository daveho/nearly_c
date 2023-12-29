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

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "location.h"
#include "node_base.h"

//! Tree node class, suitable for parse trees and ASTs.
//! Nodes can also be used as tokens returned by a lexer.
//! Note that parent nodes take responsibility for deleting
//! their children, so to delete an entire tree, it is
//! sufficient to delete the root.
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
  //! Const iterator type (for iterating through pointers to children).
  typedef std::vector<Node *>::const_iterator const_iterator;

  //! Constructor.
  //! @param tag the node tag indicating what kind of node this is
  Node(int tag);

  //! Constructor.
  //! @param tag the node tag indicating what kind of node this is
  //! @param kids initializer list with pointers to child Nodes to adopt
  Node(int tag, std::initializer_list<Node *> kids);

  //! Constructor.
  //! @param tag the node tag indicating what kind of node this is
  //! @param kids vector with pointers to child Nodes to adopt
  Node(int tag, const std::vector<Node *> &kids);

  //! Constructor for a Node with a string value (e.g., a token).
  //! @param tag the node tag indicating what kind of node this is
  //!            (i.e., the token value)
  //! @param str the node's string value (e.g., the token's lexeme)
  Node(int tag, const std::string &str);

  virtual ~Node();

  //! Get the Node's tag value.
  //! @return the Node's tag value
  int get_tag() const { return m_tag; }

  //! Set the Node's tag value.
  //! @param tag the tag value to set
  void set_tag(int tag) { m_tag = tag; }

  //! Get the Node's string value.
  //! @return the Node's string value
  std::string get_str() const { return m_str; }

  //! Set the Node's string value.
  //! @param the string value to set
  void set_str(const std::string &str) { m_str = str; }

  //! Append and adopt a child Node.
  //! @param the child Node to append and adopt
  void append_kid(Node *kid);

  //! Prepend and adopt a child Node.
  //! @param the child Node to prepend and adopt
  void prepend_kid(Node *kid);

  //! Get number of children.
  //! @return the number of children
  unsigned get_num_kids() const { return unsigned(m_kids.size()); }

  //! Get child at given index.
  //! @param index the index of the child to get (0 for first child, etc.)
  //! @return pointer to the child at given index
  Node *get_kid(unsigned index) const { return m_kids.at(index); }

  //! Get the last (righmost) child.
  //! @return poiner to the last (rightmost) child
  Node *get_last_kid() const { return m_kids.back(); }

  //! Remove the first child.
  //! Note that the discarded child is *not* deleted,
  //! so the caller must assume responsibility for deleting
  //! the removed child somehow.
  void shift_kid(); // removes the first child

  //! Replace child at given index.
  //! This is useful for restructuring the tree,
  //! but should be used with care.
  //! Note that the discarded child is *not* deleted,
  //! so the caller must assume responsibility for deleting
  //! the removed child somehow.
  //!
  //! @param index the index of the child to get (0 for first child, etc.)
  //! @param kid the child Node to adopt and place at specified index
  void set_kid(unsigned index, Node *kid) { m_kids.at(index) = kid; }

  //! Get begin iterator over pointers to children.
  //! @return begin iterator
  const_iterator cbegin() const { return m_kids.cbegin(); }

  //! Get end iterator over pointers to children.
  //! @return end iterator
  const_iterator cend() const { return m_kids.cend(); }

  //! Set this Node's source Location.
  //! @param loc the source Location to set
  void set_loc(const Location &loc) { m_loc = loc; m_loc_was_set_explicitly = true; }

  //! Get the source Location.
  //! @return the source Location
  const Location &get_loc() const { return m_loc; }

  //! Do a preorder traversal of the tree, invoking specified
  //! function on each node.
  //! @tparam Fn function type
  //! @param fn the function to apply to each tree Node in preorder
  template<typename Fn>
  void preorder(Fn fn) {
    fn(this);
    for (auto i = m_kids.begin(); i != m_kids.end(); ++i) {
      (*i)->preorder(fn);
    }
  }

  //! Invoke a function on each child.
  //! @tparam Fn function type
  //! @param fn the function to apply to each child
  template<typename Fn>
  void each_child(Fn fn) const {
    for (auto i = m_kids.begin(); i != m_kids.end(); ++i) {
      fn(*i);
    }
  }
};

#endif // NODE_H
