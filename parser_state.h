#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "location.h"
class Node;

struct ParserState {
  // To avoid depending on yyscan_t, just hard-code knowledge that
  // yyscan_t is just a typedef for void *
  void *scan_info;

  // current location (used by lexer)
  Location cur_loc;

  Node *parse_tree;
};

#endif // PARSER_STATE_H
