#include "node.h"

Node::Node(int tag)
  : m_tag(tag)
  , m_ival(0L)
  , m_loc_was_set_explicitly(false) {
}

Node::Node(int tag, std::initializer_list<Node *> kids)
  : m_tag(tag)
  , m_kids(kids)
  , m_loc_was_set_explicitly(false) {
  // parent node's location defaults to first kid's location
  if (!m_kids.empty()) {
    m_loc = m_kids[0]->get_loc();
  }
}

Node::Node(int tag, const std::vector<Node *> &kids)
  : m_tag(tag)
  , m_kids(kids)
  , m_loc_was_set_explicitly(false) {
  // parent node's location defaults to first kid's location
  if (!m_kids.empty()) {
    m_loc = m_kids[0]->get_loc();
  }
}

Node::Node(int tag, const std::string &strval)
  : m_tag(tag)
  , m_strval(strval)
  , m_ival(0L)
  , m_loc_was_set_explicitly(false) {
}

Node::Node(int tag, long ival)
  : m_tag(tag)
  , m_ival(ival)
  , m_loc_was_set_explicitly(false) {
}

Node::Node(int tag, const std::string &strval, long ival)
  : m_tag(tag)
  , m_strval(strval)
  , m_ival(ival)
  , m_loc_was_set_explicitly(false) {
}

Node::~Node() {
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
