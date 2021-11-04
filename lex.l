%{
#include "node.h"
#include "parse.tab.h"
#include "parser_state.h"

int create_token(int, const char *, YYSTYPE *, ParserState *);

// Macro to get the pointer to the ParserState from the lexer
// state, which is available (according to YY_DECL) in the
// "yyscanner" parameter to yylex()
#define PSTATE() static_cast<ParserState *>(yyget_extra(yyscanner))

// Declaration of yyerror()
void yyerror(struct ParserState *, const char *);
%}

%option noyywrap nounput reentrant bison-bridge

%%

"("                { return create_token(TOK_LPAREN, yytext, yylval, PSTATE()); }
")"                { return create_token(TOK_RPAREN, yytext, yylval, PSTATE()); }
"["                { return create_token(TOK_LBRACKET, yytext, yylval, PSTATE()); }
"]"                { return create_token(TOK_RBRACKET, yytext, yylval, PSTATE()); }
"{"                { return create_token(TOK_LBRACE, yytext, yylval, PSTATE()); }
"}"                { return create_token(TOK_RBRACE, yytext, yylval, PSTATE()); }

";"                { return create_token(TOK_SEMICOLON, yytext, yylval, PSTATE()); }
":"                { return create_token(TOK_COLON, yytext, yylval, PSTATE()); }
","                { return create_token(TOK_COMMA, yytext, yylval, PSTATE()); }
"."                { return create_token(TOK_DOT, yytext, yylval, PSTATE()); }
"?"                { return create_token(TOK_QUESTION, yytext, yylval, PSTATE()); }

"+"                { return create_token(TOK_PLUS, yytext, yylval, PSTATE()); }
"++"               { return create_token(TOK_INCREMENT, yytext, yylval, PSTATE()); }
"-"                { return create_token(TOK_MINUS, yytext, yylval, PSTATE()); }
"--"               { return create_token(TOK_DECREMENT, yytext, yylval, PSTATE()); }
"*"                { return create_token(TOK_ASTERISK, yytext, yylval, PSTATE()); }
"/"                { return create_token(TOK_DIVIDE, yytext, yylval, PSTATE()); }

"&"                { return create_token(TOK_AMPERSAND, yytext, yylval, PSTATE()); }
"|"                { return create_token(TOK_BITWISE_OR, yytext, yylval, PSTATE()); }
"^"                { return create_token(TOK_BITWISE_XOR, yytext, yylval, PSTATE()); }
"~"                { return create_token(TOK_BITWISE_COMPL, yytext, yylval, PSTATE()); }

"&&"               { return create_token(TOK_LOGICAL_AND, yytext, yylval, PSTATE()); }
"||"               { return create_token(TOK_LOGICAL_OR, yytext, yylval, PSTATE()); }

"=="               { return create_token(TOK_EQUALITY, yytext, yylval, PSTATE()); }
"!="               { return create_token(TOK_INEQUALITY, yytext, yylval, PSTATE()); }
"<"                { return create_token(TOK_LT, yytext, yylval, PSTATE()); }
"<="               { return create_token(TOK_LTE, yytext, yylval, PSTATE()); }
">"                { return create_token(TOK_GT, yytext, yylval, PSTATE()); }
">="               { return create_token(TOK_GTE, yytext, yylval, PSTATE()); }

"="                { return create_token(TOK_ASSIGN, yytext, yylval, PSTATE()); }

"if"               { return create_token(TOK_IF, yytext, yylval, PSTATE()); }
"else"             { return create_token(TOK_ELSE, yytext, yylval, PSTATE()); }
"while"            { return create_token(TOK_WHILE, yytext, yylval, PSTATE()); }
"for"              { return create_token(TOK_FOR, yytext, yylval, PSTATE()); }
"do"               { return create_token(TOK_DO, yytext, yylval, PSTATE()); }
"switch"           { return create_token(TOK_SWITCH, yytext, yylval, PSTATE()); }
"case"             { return create_token(TOK_CASE, yytext, yylval, PSTATE()); }
"char"             { return create_token(TOK_CHAR, yytext, yylval, PSTATE()); }
"short"            { return create_token(TOK_SHORT, yytext, yylval, PSTATE()); }
"int"              { return create_token(TOK_INT, yytext, yylval, PSTATE()); }
"long"             { return create_token(TOK_LONG, yytext, yylval, PSTATE()); }
"unsigned"         { return create_token(TOK_UNSIGNED, yytext, yylval, PSTATE()); }
"signed"           { return create_token(TOK_SIGNED, yytext, yylval, PSTATE()); }
"float"            { return create_token(TOK_FLOAT, yytext, yylval, PSTATE()); }
"double"           { return create_token(TOK_DOUBLE, yytext, yylval, PSTATE()); }
"void"             { return create_token(TOK_VOID, yytext, yylval, PSTATE()); }
"return"           { return create_token(TOK_RETURN, yytext, yylval, PSTATE()); }
"break"            { return create_token(TOK_BREAK, yytext, yylval, PSTATE()); }
"continue"         { return create_token(TOK_CONTINUE, yytext, yylval, PSTATE()); }
"static"           { return create_token(TOK_STATIC, yytext, yylval, PSTATE()); }
"extern"           { return create_token(TOK_EXTERN, yytext, yylval, PSTATE()); }
"auto"             { return create_token(TOK_AUTO, yytext, yylval, PSTATE()); }
"const"            { return create_token(TOK_CONST, yytext, yylval, PSTATE()); }
"volatile"         { return create_token(TOK_VOLATILE, yytext, yylval, PSTATE()); }
"struct"           { return create_token(TOK_STRUCT, yytext, yylval, PSTATE()); }
"union"            { return create_token(TOK_UNION, yytext, yylval, PSTATE()); }

[A-Za-z_][A-Za-z_0-9]* { return create_token(TOK_IDENT, yytext, yylval, PSTATE()); }

[ \t\r]+           { PSTATE()->cur_loc.advance(int(yyleng)); }
\n                 { PSTATE()->cur_loc.next_line(); }

.                  { yyerror(PSTATE(), "Unrecognized character"); }


%%

int create_token(int token_tag, const char *lexeme, YYSTYPE *semantic_value, ParserState *pp) {
  Node *tok = new Node(token_tag, lexeme);
  tok->set_loc(pp->cur_loc);

  semantic_value->node = tok;

  pp->cur_loc.advance(int(tok->get_strval().size()));

  printf("read token: %s(%d)\n", lexeme, token_tag);

  return token_tag;
}
