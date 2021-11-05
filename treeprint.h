#ifndef TREEPRINT_H
#define TREEPRINT_H

#include <string>
struct Node;

class TreePrint {
public:
  TreePrint();
  virtual ~TreePrint();

  void print(Node *t) const;

  virtual std::string node_tag_to_string(int tag) const = 0;
};

#endif // TREEPRINT_H
