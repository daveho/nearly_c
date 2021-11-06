#include "node.h"

Node::Node(int tag)
  : m_tag(tag)
  , m_ival(0L) {
}

Node::Node(int tag, std::initializer_list<Node *> kids)
  : m_tag(tag)
  , m_kids(kids) {
}

Node::Node(int tag, const std::string &strval)
  : m_tag(tag)
  , m_strval(strval)
  , m_ival(0L) {
}

Node::Node(int tag, long ival)
  : m_tag(tag)
  , m_ival(ival) {
}

Node::Node(int tag, const std::string &strval, long ival)
  : m_tag(tag)
  , m_strval(strval)
  , m_ival(ival) {
}

Node::~Node() {
}
