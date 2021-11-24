#include <memory>
#include <cassert>
#include "exceptions.h"
#include "node.h"
#include "parse.tab.h"
#include "lex.yy.h"
#include "parser_state.h"
#include "arena.h"
#include "context.h"

Context::Context()
  : m_ast_arena(new ChunkedArena)
  , m_ast(nullptr) {
}

Context::~Context() {
  delete m_ast_arena;
}

struct CloseFile {
  void operator()(FILE *in) {
    if (in != nullptr) {
      fclose(in);
    }
  }
};

void Context::parse(const std::string &filename) {
  // open the input source file
  std::unique_ptr<FILE, CloseFile> in(fopen(filename.c_str(), "r"));
  if (!in) {
    RuntimeError::raise("Couldn't open '%s'", filename.c_str());
  }

  // create an initialize ParserState; note that its destructor
  // will take responsibility for cleaning up the lexer state
  std::unique_ptr<ParserState> pp(new ParserState);
  pp->cur_loc = Location(filename, 1, 1);

  // use an Arena to allocate AST objects
  pp->arena = m_ast_arena;

  // prepare the lexer
  yylex_init(&pp->scan_info);
  yyset_in(in.get(), pp->scan_info);

  // make the ParserState available from the lexer state
  yyset_extra(pp.get(), pp->scan_info);

  // parse the input source code
  yyparse(pp.get());

  m_ast = pp->parse_tree;
}
