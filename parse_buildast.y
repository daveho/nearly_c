%{
// Copyright (c) 2021, David H. Hovemeyer <david.hovemeyer@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// parse.y
// This is the parser that builds ASTs

#include "node.h"
#include "parser_state.h"
#include "grammar_symbols.h"
#include "ast.h"
#include "yyerror.h"

// This is a weird hack required to make bison pass the
// yyscan_t value representing the scanner state (which is
// stored in the ParserState object) to yylex()
#define the_scanner pp->scan_info

// Bison does not actually declare yylex()
typedef union YYSTYPE YYSTYPE;
int yylex(YYSTYPE *, void *);

namespace {
  // This function is called to add a TOK_UNSPECIFIED_STORAGE token
  // to the beginning of a declaration where the storage class was not
  // specified explicitly.
  void handle_unspecified_storage(Node *ast, struct ParserState *pp) {
    Node *first_kid = ast->get_kid(0);
    Node *unspecified_storage = pp->arena->create<Node>(NODE_TOK_UNSPECIFIED_STORAGE);
    unspecified_storage->set_loc(first_kid->get_loc());
    ast->prepend_kid(unspecified_storage);
  }
}
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

  /*
   * We expect one shift/reduce conflict due to the "dangling else" problem.
   * Favoring shifting (which is what bison will do) is the correct
   * resolution.
   */
%expect 1

%union {
  Node *node;
}

%token<node> TOK_LPAREN TOK_RPAREN TOK_LBRACKET TOK_RBRACKET TOK_LBRACE TOK_RBRACE
%token<node> TOK_SEMICOLON TOK_COLON
%token<node> TOK_COMMA TOK_DOT TOK_QUESTION TOK_NOT

%token<node> TOK_PLUS TOK_INCREMENT TOK_MINUS TOK_DECREMENT
%token<node> TOK_ASTERISK TOK_DIVIDE TOK_MOD

%token<node> TOK_AMPERSAND TOK_BITWISE_OR TOK_BITWISE_XOR TOK_BITWISE_COMPL
%token<node> TOK_LEFT_SHIFT TOK_RIGHT_SHIFT

%token<node> TOK_LOGICAL_AND TOK_LOGICAL_OR

%token<node> TOK_EQUALITY TOK_INEQUALITY TOK_LT TOK_LTE TOK_GT TOK_GTE

%token<node> TOK_ASSIGN TOK_MUL_ASSIGN TOK_DIV_ASSIGN TOK_MOD_ASSIGN TOK_ADD_ASSIGN
%token<node> TOK_SUB_ASSIGN TOK_LEFT_ASSIGN TOK_RIGHT_ASSIGN TOK_AND_ASSIGN TOK_XOR_ASSIGN
%token<node> TOK_OR_ASSIGN

%token<node> TOK_IF TOK_ELSE TOK_WHILE TOK_FOR TOK_DO TOK_SWITCH TOK_CASE
%token<node> TOK_CHAR TOK_SHORT TOK_INT TOK_LONG TOK_UNSIGNED TOK_SIGNED
%token<node> TOK_FLOAT TOK_DOUBLE
%token<node> TOK_VOID
%token<node> TOK_RETURN TOK_BREAK TOK_CONTINUE
%token<node> TOK_CONST TOK_VOLATILE
%token<node> TOK_STRUCT TOK_UNION

  /*
   * Storage class specifiers: because storage class is optional,
   * the AST building parser adds TOK_UNSPECIFIED_STORAGE to the AST
   * for declarations where the storage class isn't specified.
   * This makes things easier for consumers of the AST, because
   * they can assume that a storage class is present for
   * all declarations.
   *
   * The parse-tree-building parser (parse.y) does not use
   * TOK_UNSPECIFIED_STORAGE, and it will never appear in a parse tree.
   */
%token<node> TOK_UNSPECIFIED_STORAGE
%token<node> TOK_STATIC TOK_EXTERN TOK_AUTO

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
%type<node> assignment_expression assignment_op
%type<node> conditional_expression logical_or_expression logical_and_expression
%type<node> bitwise_and_expression bitwise_or_expression bitwise_xor_expression
%type<node> equality_expression relational_expression relational_op
%type<node> shift_expression additive_expression multiplicative_expression
%type<node> cast_expression unary_expression postfix_expression primary_expression
%type<node> argument_expression_list

%%

unit
  : top_level_declaration
   { pp->parse_tree = $$ = pp->arena->create<Node>(AST_UNIT, Node::I{$1}); }
  | top_level_declaration unit
    { pp->parse_tree = $$ = $2; $$->prepend_kid($1); }
  ;

top_level_declaration
  : function_or_variable_declaration_or_definition
    { $$ = $1; handle_unspecified_storage($$, pp); }
  | TOK_STATIC function_or_variable_declaration_or_definition
    { $$ = $2; $$->prepend_kid($1); }
  | TOK_EXTERN function_or_variable_declaration_or_definition
    { $$ = $2; $$->prepend_kid($1); }
  | struct_type_definition
    { $$ = $1; }
  | union_type_definition
    { $$ = $1; }
  ;

function_or_variable_declaration_or_definition
  : function_definition_or_declaration
    { $$ = $1; }
  | simple_variable_declaration
    { $$ = $1; }
  ;

simple_variable_declaration
  : type declarator_list TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_VARIABLE_DECLARATION, Node::I {$1, $2}); }
  ;

declarator_list
  : declarator
    { $$ = pp->arena->create<Node>(AST_DECLARATOR_LIST, Node::I {$1}); }
  | declarator TOK_COMMA declarator_list
    { $$ = $3; $$->prepend_kid($1); }
  ;

  /* for now, only variables can be declared */
declarator
  : TOK_IDENT
    { $$ = pp->arena->create<Node>(AST_NAMED_DECLARATOR, Node::I {$1}); }
  ;

function_definition_or_declaration
  : type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_LBRACE opt_statement_list TOK_RBRACE
    { $$ = pp->arena->create<Node>(AST_FUNCTION_DEFINITION, Node::I {$1, $2, $4, $7}); }
  | type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_FUNCTION_DECLARATION, Node::I {$1, $2, $4}); }
  ;

function_parameter_list
  : TOK_VOID
    { $$ = pp->arena->create<Node>(AST_FUNCTION_PARAMETER_LIST); }
  | opt_parameter_list
    { $$ = $1; }
  ;

opt_parameter_list
  : parameter_list
    { $$ = $1; }
  | /* nothing */
    { $$ = pp->arena->create<Node>(AST_FUNCTION_PARAMETER_LIST); }
  ;

parameter_list
  : parameter
    { $$ = pp->arena->create<Node>(AST_FUNCTION_PARAMETER_LIST, Node::I {$1}); }
  | parameter TOK_COMMA parameter_list
    { $$ = $3; $$->prepend_kid($1); }
  ;

parameter
  : type declarator
    { $$ = pp->arena->create<Node>(AST_FUNCTION_PARAMETER, Node::I {$1, $2}); }
  ;

type
  : basic_type
    { $$ = $1; }
  | TOK_STRUCT TOK_IDENT
    { $$ = pp->arena->create<Node>(AST_STRUCT_TYPE, Node::I {$2}); }
  | TOK_UNION TOK_IDENT
    { $$ = pp->arena->create<Node>(AST_UNION_TYPE, Node::I {$2}); }
  ;

  /*
   * C essentially allows a salad of type keywords to
   * be combined, so we'll do that too.
   * Semantic analysis can make sense of them later.
   */
basic_type
  : basic_type_keyword
    { $$ = pp->arena->create<Node>(AST_BASIC_TYPE, Node::I {$1}); }
  | basic_type_keyword basic_type
    { $$ = $2; $$->prepend_kid($1); }
  ;

basic_type_keyword
  : TOK_CHAR
    { $$ = $1; }
  | TOK_SHORT
    { $$ = $1; }
  | TOK_INT
    { $$ = $1; }
  | TOK_LONG
    { $$ = $1; }
  | TOK_UNSIGNED
    { $$ = $1; }
  | TOK_SIGNED
    { $$ = $1; }
  | TOK_FLOAT
    { $$ = $1; }
  | TOK_DOUBLE
    { $$ = $1; }
  | TOK_VOID
    { $$ = $1; }
  | TOK_CONST
    { $$ = $1; }
  | TOK_VOLATILE
    { $$ = $1; }
  ;

opt_statement_list
  : statement_list
    { $$ = $1; }
  | /* nothing */
    { $$ = pp->arena->create<Node>(AST_STATEMENT_LIST); }
  ;

statement_list
  : statement
    { $$ = pp->arena->create<Node>(AST_STATEMENT_LIST, Node::I {$1}); }
  | statement statement_list
    { $$ = $2; $$->prepend_kid($1); }
  ;

statement
  : TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_EMPTY_STATEMENT); }
  | simple_variable_declaration
    { $$ = $1; handle_unspecified_storage($$, pp); }
  | TOK_STATIC simple_variable_declaration
    { $$ = $2; $$->prepend_kid($1); }
  | TOK_EXTERN simple_variable_declaration
    { $$ = $2; $$->prepend_kid($1); }
  | assignment_expression TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_EXPRESSION_STATEMENT, Node::I {$1}); }
  | TOK_RETURN TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_RETURN_STATEMENT); }
  | TOK_RETURN assignment_expression TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_RETURN_EXPRESSION_STATEMENT, Node::I {$2}); }
  | TOK_LBRACE opt_statement_list TOK_RBRACE
    { $$ = $2;  }
  | TOK_WHILE TOK_LPAREN assignment_expression TOK_RPAREN statement
    { $$ = pp->arena->create<Node>(AST_WHILE_STATEMENT, Node::I {$3, $5}); }
  | TOK_DO statement TOK_WHILE TOK_LPAREN assignment_expression TOK_RPAREN TOK_SEMICOLON
    { $$ = pp->arena->create<Node>(AST_DO_WHILE_STATEMENT, Node::I {$2, $5}); }
    /*
     * TODO: allow variable definition in a for loop initializer,
     * and also allow initialization, loop condition, and/or update
     * to be omitted
     */
  | TOK_FOR TOK_LPAREN assignment_expression TOK_SEMICOLON
                       assignment_expression TOK_SEMICOLON
                       assignment_expression TOK_RPAREN statement
    { $$ = pp->arena->create<Node>(AST_FOR_STATEMENT, Node::I {$3, $5, $7, $9}); }
  | TOK_IF TOK_LPAREN assignment_expression TOK_RPAREN statement
    { $$ = pp->arena->create<Node>(AST_IF_STATEMENT, Node::I {$3, $5}); }
  | TOK_IF TOK_LPAREN assignment_expression TOK_RPAREN statement TOK_ELSE statement
    { $$ = pp->arena->create<Node>(AST_IF_ELSE_STATEMENT, Node::I {$3, $5, $7}); }
  ;

struct_type_definition
  : TOK_STRUCT TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
    { $$ = pp->arena->create<Node>(AST_STRUCT_TYPE_DEFINITION, Node::I {$4}); }
  ;

union_type_definition
  : TOK_UNION TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE
    { $$ = pp->arena->create<Node>(AST_UNION_TYPE_DEFINITION, Node::I {$4}); }
  ;

opt_simple_variable_declaration_list
  : simple_variable_declaration_list
    { $$ = $1; }
  | /* nothing */
    { $$ = pp->arena->create<Node>(AST_FIELD_DEFINITION_LIST); }
  ;

simple_variable_declaration_list
  : simple_variable_declaration
    { $$ = pp->arena->create<Node>(AST_FIELD_DEFINITION_LIST, Node::I {$1}); }
  | simple_variable_declaration simple_variable_declaration_list
    { $$ = $2; $$->prepend_kid($1); }
  ;

  /*
   * Expression grammar rules adapted from
   * https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
   */

  /*
   * Note: when building expression ASTs, the operator will
   * always be the first child, and the operand or operands
   * will follow the operator.  The node tag for an expression
   * will always be either AST_BINARY_EXPRESSION or AST_UNARY_EXPRESSION.
   * This approach avoids creating a proliferation of AST node
   * types for different kinds of expressions.
   */

assignment_expression
  : unary_expression assignment_op assignment_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | conditional_expression
    { $$ = $1; }
  ;

assignment_op
  : TOK_ASSIGN
    { $$ = $1; }
  | TOK_MUL_ASSIGN
    { $$ = $1; }
  | TOK_DIV_ASSIGN
    { $$ = $1; }
  | TOK_MOD_ASSIGN
    { $$ = $1; }
  | TOK_ADD_ASSIGN
    { $$ = $1; }
  | TOK_SUB_ASSIGN
    { $$ = $1; }
  | TOK_LEFT_ASSIGN
    { $$ = $1; }
  | TOK_RIGHT_ASSIGN
    { $$ = $1; }
  | TOK_AND_ASSIGN
    { $$ = $1; }
  | TOK_XOR_ASSIGN
    { $$ = $1; }
  | TOK_OR_ASSIGN
    { $$ = $1; }
  ;

conditional_expression
  : logical_or_expression
    { $$ = $1; }
  | logical_or_expression TOK_QUESTION assignment_expression TOK_COLON conditional_expression
    { $$ = pp->arena->create<Node>(AST_CONDITIONAL_EXPRESSION, Node::I {$1, $3, $5}); }
  ;

logical_or_expression
  : logical_and_expression
    { $$ = $1; }
  | logical_or_expression TOK_LOGICAL_OR logical_and_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

logical_and_expression
  : bitwise_or_expression
    { $$ = $1; }
  | logical_and_expression TOK_LOGICAL_AND bitwise_or_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

bitwise_or_expression
  : bitwise_xor_expression
    { $$ = $1; }
  | bitwise_or_expression TOK_BITWISE_OR bitwise_xor_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

bitwise_xor_expression
  : bitwise_and_expression
    { $$ = $1; }
  | bitwise_xor_expression TOK_BITWISE_XOR bitwise_and_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

bitwise_and_expression
  : equality_expression
    { $$ = $1; }
  | bitwise_and_expression TOK_AMPERSAND equality_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

equality_expression
  : relational_expression
    { $$ = $1; }
  | equality_expression TOK_EQUALITY relational_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | equality_expression TOK_INEQUALITY relational_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

relational_expression
  : shift_expression
    { $$ = $1; }
  | relational_expression relational_op shift_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

relational_op
  : TOK_LT
    { $$ = $1; }
  | TOK_LTE
    { $$ = $1; }
  | TOK_GT
    { $$ = $1; }
  | TOK_GTE
    { $$ = $1; }
  ;

shift_expression
  : additive_expression
    { $$ = $1; }
  | shift_expression TOK_LEFT_SHIFT additive_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | shift_expression TOK_RIGHT_SHIFT additive_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

additive_expression
  : multiplicative_expression
    { $$ = $1; }
  | additive_expression TOK_PLUS multiplicative_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | additive_expression TOK_MINUS multiplicative_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

multiplicative_expression
  : cast_expression
    { $$ = $1; }
  | multiplicative_expression TOK_ASTERISK cast_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | multiplicative_expression TOK_DIVIDE cast_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  | multiplicative_expression TOK_MOD cast_expression
    { $$ = pp->arena->create<Node>(AST_BINARY_EXPRESSION, Node::I {$2, $1, $3}); }
  ;

cast_expression
  : unary_expression
    { $$ = $1; }
  | TOK_LPAREN type TOK_RPAREN cast_expression
    { $$ = pp->arena->create<Node>(AST_CAST_EXPRESSION, Node::I {$1, $2}); }
  ;

unary_expression
  : postfix_expression
    { $$ = $1; }
  | TOK_PLUS cast_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  | TOK_MINUS cast_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  | TOK_NOT cast_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  | TOK_BITWISE_COMPL cast_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  | TOK_INCREMENT unary_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  | TOK_DECREMENT unary_expression
    { $$ = pp->arena->create<Node>(AST_UNARY_EXPRESSION, Node::I {$1, $2}); }
  ;

postfix_expression
  : primary_expression
    { $$ = $1; }
  | postfix_expression TOK_INCREMENT
    { $$ = pp->arena->create<Node>(AST_POSTFIX_EXPRESSION, Node::I {$2, $1}); }
  | postfix_expression TOK_DECREMENT
    { $$ = pp->arena->create<Node>(AST_POSTFIX_EXPRESSION, Node::I {$2, $1}); }
  | postfix_expression TOK_LPAREN TOK_RPAREN
    { $$ = pp->arena->create<Node>(AST_FUNCTION_CALL_EXPRESSION, Node::I {$1}); }
  | postfix_expression TOK_LPAREN argument_expression_list TOK_RPAREN
    { $$ = pp->arena->create<Node>(AST_FUNCTION_CALL_EXPRESSION, Node::I {$1, $3}); }
  | postfix_expression TOK_DOT TOK_IDENT
    { $$ = pp->arena->create<Node>(AST_FIELD_REFERENCE_EXPRESSION, Node::I {$1, $3}); }
  ;

argument_expression_list
  : assignment_expression
    { $$ = pp->arena->create<Node>(AST_ARGUMENT_EXPRESSION_LIST, Node::I {$1}); }
  | assignment_expression TOK_COMMA argument_expression_list
    { $$ = $3; $$->prepend_kid($1); }
  ;

primary_expression
  : TOK_INT_LIT
    { $$ = pp->arena->create<Node>(AST_LITERAL_VALUE, Node::I {$1}); }
  | TOK_CHAR_LIT
    { $$ = pp->arena->create<Node>(AST_LITERAL_VALUE, Node::I {$1}); }
  | TOK_FP_LIT
    { $$ = pp->arena->create<Node>(AST_LITERAL_VALUE, Node::I {$1}); }
  | TOK_STR_LIT
    { $$ = pp->arena->create<Node>(AST_LITERAL_VALUE, Node::I {$1}); }
  | TOK_IDENT
    { $$ = pp->arena->create<Node>(AST_VARIABLE_REFERENCE, Node::I {$1}); }
  | TOK_LPAREN assignment_expression TOK_RPAREN
    { $$ = $2; }
  ;

%%
