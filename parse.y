%{
#include "node.h"
#include "parser_state.h"

// This is a weird hack required to make bison pass the
// yyscan_t value representing the scanner state (which is
// stored in the ParserState object) to yylex()
#define the_scanner pp->scan_info

// Bison does not actually declare yylex()
typedef union YYSTYPE YYSTYPE;
int yylex(YYSTYPE *, void *);

// Nor does it declare yyerror()
void yyerror(struct ParserState *, const char *);
%}

%define api.pure

  /*
   * A ParserState object has all of the state for the lexer
   * and parser
   */
%parse-param { struct ParserState *pp }

  /*
   * Note that lex-param only controls the actual argument to yylex,
   * and can only consist of a single token (hence the need to make
   * it a #define so the lexer state can be accessed within the
   * ParserState object
   */
%lex-param { the_scanner }

%union {
  Node *node;
}

%token<node> TOK_LPAREN TOK_RPAREN TOK_LBRACKET TOK_RBRACKET TOK_LBRACE TOK_RBRACE
%token<node> TOK_SEMICOLON TOK_COLON
%token<node> TOK_COMMA TOK_DOT TOK_QUESTION

%token<node> TOK_PLUS TOK_INCREMENT TOK_MINUS TOK_DECREMENT
%token<node> TOK_ASTERISK TOK_DIVIDE

%token<node> TOK_AMPERSAND TOK_BITWISE_OR TOK_BITWISE_XOR TOK_BITWISE_COMPL
%token<node> TOK_LOGICAL_AND TOK_LOGICAL_OR

%token<node> TOK_EQUALITY TOK_INEQUALITY TOK_LT TOK_LTE TOK_GT TOK_GTE

%token<node> TOK_ASSIGN

%token<node> TOK_IF TOK_ELSE TOK_WHILE TOK_FOR TOK_DO TOK_SWITCH TOK_CASE
%token<node> TOK_CHAR TOK_SHORT TOK_INT TOK_LONG TOK_UNSIGNED TOK_SIGNED
%token<node> TOK_FLOAT TOK_DOUBLE
%token<node> TOK_VOID
%token<node> TOK_RETURN TOK_BREAK TOK_CONTINUE
%token<node> TOK_STATIC TOK_EXTERN TOK_AUTO TOK_CONST TOK_VOLATILE
%token<node> TOK_STRUCT TOK_UNION

%token<node> TOK_IDENT

%%

unit
  : top_level_declaration
  | top_level_declaration unit
  ;

top_level_declaration
  : variable_declaration
  | function_declaration
  | function_definition
  | struct_type_definition
  | union_type_definition
  ;

variable_declaration
  : opt_storage_class simple_variable_declaration { printf("variable_declaration\n"); }
  ;

simple_variable_declaration
  : opt_type_qualifier type identifier_list { printf("simple_variable_declaration\n"); }
  ;

identifier_list
  : TOK_IDENT { printf("identifier_list\n"); }
  | TOK_IDENT TOK_COMMA identifier_list
  ;

function_declaration
  : type TOK_IDENT TOK_LPAREN parameter_list TOK_RPAREN TOK_SEMICOLON
  ;

opt_storage_class
  : TOK_STATIC
  | TOK_EXTERN
  | /* nothing */ { printf("opt_storage_class\n"); }
  ;

opt_type_qualifier
  : TOK_CONST
  | TOK_VOLATILE
  | /* nothing */ { printf("opt_type_qualifier\n"); }
  ;

parameter_list
  : TOK_VOID
  ;

function_definition
  : type TOK_IDENT TOK_LPAREN parameter_list TOK_RPAREN TOK_LBRACE opt_statement_list TOK_RBRACE
  ;

type
  : basic_type { printf("type\n"); }
  | TOK_STRUCT TOK_IDENT
  | TOK_UNION TOK_IDENT
  ;

  /*
   * C essentially allows a salad of type keywords to
   * be combined, so we'll do that too.
   * Semantic analysis can make sense of them later.
   */
basic_type
  : basic_type_keyword { printf("basic_type\n"); }
  | basic_type_keyword basic_type
  ;

basic_type_keyword
  : TOK_CHAR
  | TOK_SHORT
  | TOK_INT { printf("basic_type_keyword\n"); }
  | TOK_LONG
  | TOK_UNSIGNED
  | TOK_SIGNED
  | TOK_FLOAT
  | TOK_DOUBLE
  | TOK_VOID
  ;

opt_statement_list
  : statement_list
  | /* nothing */
  ;

statement_list
  : statement
  | statement statement_list
  ;

statement
  : TOK_SEMICOLON
  ;

struct_type_definition
  : TOK_STRUCT TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
  ;

union_type_definition
  : TOK_UNION TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
  ;

opt_simple_variable_declaration_list
  : simple_variable_declaration_list
  | /* nothing */
  ;

simple_variable_declaration_list
  : simple_variable_declaration
  | simple_variable_declaration simple_variable_declaration_list
  ;

%%

void yyerror(struct ParserState *pp, const char *msg) {
  // FIXME: throw exception?
  // FIXME: report location
  fprintf(stderr, "Error: %s\n", msg);
  exit(1);
}
