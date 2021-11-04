#ifndef PARSER_STATE_H
#define PARSER_STATE_H

struct ParserState {
  // To avoid depending on yyscan_t, just hard-code knowledge that
  // yyscan_t is just a typedef for void *
  void *scan_info;
};

#endif // PARSER_STATE_H
