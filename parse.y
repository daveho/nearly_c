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
  : function_or_variable_declaration_or_definition
  | TOK_STATIC function_or_variable_declaration_or_definition
  | TOK_EXTERN function_or_variable_declaration_or_definition
  | struct_type_definition
  | union_type_definition
  ;

function_or_variable_declaration_or_definition
  : function_definition_or_declaration
  | simple_variable_declaration
  ;

simple_variable_declaration
  : type declarator_list TOK_SEMICOLON
  ;

declarator_list
  : declarator
  | declarator TOK_COMMA declarator_list
  ;

  /* for now, only variables can be declared */
declarator
  : TOK_IDENT
  ;

function_definition_or_declaration
  : type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_LBRACE opt_statement_list TOK_RBRACE
  | type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_SEMICOLON
  ;

function_parameter_list
  : TOK_VOID
  | opt_parameter_list
  ;

opt_parameter_list
  : parameter_list
  | /* nothing */
  ;

parameter_list
  : simple_variable_declaration
  | simple_variable_declaration TOK_COMMA parameter_list
  ;

type
  : basic_type
  | TOK_STRUCT TOK_IDENT
  | TOK_UNION TOK_IDENT
  ;

  /*
   * C essentially allows a salad of type keywords to
   * be combined, so we'll do that too.
   * Semantic analysis can make sense of them later.
   */
basic_type
  : basic_type_keyword
  | basic_type_keyword basic_type
  ;

basic_type_keyword
  : TOK_CHAR
  | TOK_SHORT
  | TOK_INT
  | TOK_LONG
  | TOK_UNSIGNED
  | TOK_SIGNED
  | TOK_FLOAT
  | TOK_DOUBLE
  | TOK_VOID
  | TOK_CONST
  | TOK_VOLATILE
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
  | simple_variable_declaration
  | TOK_STATIC simple_variable_declaration
  | TOK_EXTERN simple_variable_declaration
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
