#include "node.h"

Node::Node(int tag)
  : m_tag(tag)
  , m_ival(0L) {
}

Node::Node(int tag, Node *kid1)
  : Node(tag) {
  append_kid(kid1);
}

Node::Node(int tag, Node *kid1, Node *kid2)
  : Node(tag, kid1) {
  append_kid(kid2);
}

Node::Node(int tag, Node *kid1, Node *kid2, Node *kid3)
  : Node(tag, kid1, kid2) {
  append_kid(kid3);
}

Node::Node(int tag, Node *kid1, Node *kid2, Node *kid3, Node *kid4)
  : Node(tag, kid1, kid2, kid3) {
  append_kid(kid4);
}

Node::Node(int tag, Node *kid1, Node *kid2, Node *kid3, Node *kid4, Node *kid5)
  : Node(tag, kid1, kid2, kid3, kid4) {
  append_kid(kid5);
}

Node::Node(int tag, Node *kid1, Node *kid2, Node *kid3, Node *kid4, Node *kid5, Node *kid6)
  : Node(tag, kid1, kid2, kid3, kid4, kid5) {
  append_kid(kid6);
}

Node::Node(int tag, Node *kid1, Node *kid2, Node *kid3, Node *kid4, Node *kid5, Node *kid6, Node *kid7)
  : Node(tag, kid1, kid2, kid3, kid4, kid5, kid6) {
  append_kid(kid7);
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
