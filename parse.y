%{
#include "node.h"
#include "parser_state.h"
#include "grammar_symbols.h"

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

%token<node> TOK_STR_LIT TOK_CHAR_LIT TOK_INT_LIT TOK_FP_LIT

%type<node> unit top_level_declaration function_or_variable_declaration_or_definition
%type<node> simple_variable_declaration
%type<node> declarator_list declarator
%type<node> function_definition_or_declaration
%type<node> function_parameter_list opt_parameter_list parameter_list parameter
%type<node> type basic_type basic_type_keyword
%type<node> opt_statement_list statement_list statement
%type<node> struct_type_definition union_type_definition
%type<node> opt_simple_variable_declaration_list simple_variable_declaration_list

%%

unit
  : top_level_declaration
   { pp->parse_tree = $$ = new Node(NODE_unit, {$1}); }
  | top_level_declaration unit
    { pp->parse_tree = $$ = new Node(NODE_unit, {$1, $2}); }
  ;

top_level_declaration
  : function_or_variable_declaration_or_definition
    { $$ = new Node(NODE_top_level_declaration, {$1}); }
  | TOK_STATIC function_or_variable_declaration_or_definition
    { $$ = new Node(NODE_top_level_declaration, {$1, $2}); }
  | TOK_EXTERN function_or_variable_declaration_or_definition
    { $$ = new Node(NODE_top_level_declaration, {$1, $2}); }
  | struct_type_definition
    { $$ = new Node(NODE_top_level_declaration, {$1}); }
  | union_type_definition
    { $$ = new Node(NODE_top_level_declaration, {$1}); }
  ;

function_or_variable_declaration_or_definition
  : function_definition_or_declaration
    { $$ = new Node(NODE_function_or_variable_declaration_or_definition, {$1}); }
  | simple_variable_declaration
    { $$ = new Node(NODE_function_or_variable_declaration_or_definition, {$1}); }
  ;

simple_variable_declaration
  : type declarator_list TOK_SEMICOLON
    { $$ = new Node(NODE_simple_variable_declaration, {$1, $2}); }
  ;

declarator_list
  : declarator
    { $$ = new Node(NODE_declarator_list, {$1}); }
  | declarator TOK_COMMA declarator_list
    { $$ = new Node(NODE_declarator_list, {$1, $2, $3}); }
  ;

  /* for now, only variables can be declared */
declarator
  : TOK_IDENT
    { $$ = new Node(NODE_declarator, {$1}); }
  ;

function_definition_or_declaration
  : type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_LBRACE opt_statement_list TOK_RBRACE
    { $$ = new Node(NODE_function_definition_or_declaration, {$1, $2, $3, $4, $5, $6, $7, $8}); }
  | type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_SEMICOLON
    { $$ = new Node(NODE_function_definition_or_declaration, {$1, $2, $3, $4, $5, $6}); }
  ;

function_parameter_list
  : TOK_VOID
    { $$ = new Node(NODE_function_parameter_list, {$1}); }
  | opt_parameter_list
    { $$ = new Node(NODE_opt_parameter_list); }
  ;

opt_parameter_list
  : parameter_list
    { $$ = new Node(NODE_opt_parameter_list, {$1}); }
  | /* nothing */
    { $$ = new Node(NODE_opt_parameter_list); }
  ;

parameter_list
  : parameter
    { $$ = new Node(NODE_parameter_list); }
  | parameter TOK_COMMA parameter_list
    { $$ = new Node(NODE_parameter_list, {$1, $2, $3}); }
  ;

parameter
  : type TOK_IDENT
    { $$ = new Node(NODE_parameter, {$1, $2}); }
  ;

type
  : basic_type
    { $$ = new Node(NODE_type, {$1}); }
  | TOK_STRUCT TOK_IDENT
    { $$ = new Node(NODE_type, {$1, $2}); }
  | TOK_UNION TOK_IDENT
    { $$ = new Node(NODE_type, {$1, $2}); }
  ;

  /*
   * C essentially allows a salad of type keywords to
   * be combined, so we'll do that too.
   * Semantic analysis can make sense of them later.
   */
basic_type
  : basic_type_keyword
    { $$ = new Node(NODE_basic_type, {$1}); }
  | basic_type_keyword basic_type
    { $$ = new Node(NODE_basic_type, {$1, $2}); }
  ;

basic_type_keyword
  : TOK_CHAR
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_SHORT
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_INT
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_LONG
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_UNSIGNED
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_SIGNED
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_FLOAT
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_DOUBLE
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_VOID
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_CONST
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  | TOK_VOLATILE
    { $$ = new Node(NODE_basic_type_keyword, {$1}); }
  ;

opt_statement_list
  : statement_list
    { $$ = new Node(NODE_opt_statement_list, {$1}); }
  | /* nothing */
    { $$ = new Node(NODE_opt_statement_list); }
  ;

statement_list
  : statement
    { $$ = new Node(NODE_statement_list, {$1}); }
  | statement statement_list
    { $$ = new Node(NODE_statement_list, {$1, $2}); }
  ;

statement
  : TOK_SEMICOLON
    { $$ = new Node(NODE_statement, {$1}); }
  | simple_variable_declaration
    { $$ = new Node(NODE_statement, {$1}); }
  | TOK_STATIC simple_variable_declaration
    { $$ = new Node(NODE_statement, {$1, $2}); }
  | TOK_EXTERN simple_variable_declaration
    { $$ = new Node(NODE_statement, {$1, $2}); }
  ;

struct_type_definition
  : TOK_STRUCT TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
    { $$ = new Node(NODE_struct_type_definition, {$1, $2, $3, $4, $5}); }
  ;

union_type_definition
  : TOK_UNION TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
    { $$ = new Node(NODE_union_type_definition, {$1, $2, $3, $4, $5}); }
  ;

opt_simple_variable_declaration_list
  : simple_variable_declaration_list
    { $$ = new Node(NODE_opt_simple_variable_declaration_list, {$1}); }
  | /* nothing */
    { $$ = new Node(NODE_opt_simple_variable_declaration_list); }
  ;

simple_variable_declaration_list
  : simple_variable_declaration
    { $$ = new Node(NODE_simple_variable_declaration_list, {$1}); }
  | simple_variable_declaration simple_variable_declaration_list
    { $$ = new Node(NODE_simple_variable_declaration_list, {$1, $2}); }
  ;

%%

void yyerror(struct ParserState *pp, const char *msg) {
  // FIXME: throw exception?
  // FIXME: report location
  fprintf(stderr, "Error: %s\n", msg);
  exit(1);
}
