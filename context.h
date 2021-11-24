#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
class Node;
class Arena;

// The Context class gathers together all of the objects/data
// used in the compilation process, and orchestrates the various
// passes and transformations.
class Context {
private:
  Arena *m_ast_arena; // Arena for AST objects
  Node *m_ast;

  // copy ctor and assignment operator not allowed
  Context(const Context &);
  Context &operator=(const Context &);

public:
  Context();
  ~Context();

  // Parse an input file and build an AST
  void parse(const std::string &filename);

  // Get pointer to root of AST
  Node *get_ast() const { return m_ast; }

  // TODO: add member functions for semantic analysis, code generation, etc.
};

#endif // CONTEXT_H
