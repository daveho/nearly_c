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

  // no value semantics
  Node(const Node &);
  Node &operator=(const Node &);

public:
  typedef std::vector<Node *>::const_iterator const_iterator;

  Node(int tag);
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

  const_iterator cbegin() const { return m_kids.cbegin(); }
  const_iterator cend() const { return m_kids.cend(); }

  void set_loc(const Location &loc) { m_loc = loc; }
  const Location &get_loc() const { return m_loc; }
};

#endif // NODE_H
