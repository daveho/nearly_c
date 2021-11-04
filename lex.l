%{

#include "node.h"
#include "parse.tab.h"

//Location cur_loc;

int create_token(int, const char *, YYSTYPE *);
%}

%option noyywrap nounput reentrant bison-bridge

%%

"("                                { return create_token(TOK_LPAREN, yytext, yylval); }
")"                                { return create_token(TOK_RPAREN, yytext, yylval); }
"["                                { return create_token(TOK_LBRACKET, yytext, yylval); }
"]"                                { return create_token(TOK_RBRACKET, yytext, yylval); }
"{"                                { return create_token(TOK_LBRACE, yytext, yylval); }
"}"                                { return create_token(TOK_RBRACE, yytext, yylval); }

";"                                { return create_token(TOK_SEMICOLON, yytext, yylval); }
":"                                { return create_token(TOK_COLON, yytext, yylval); }

"+"                                { return create_token(TOK_PLUS, yytext, yylval); }
"++"                               { return create_token(TOK_INCREMENT, yytext, yylval); }
"-"                                { return create_token(TOK_MINUS, yytext, yylval); }
"--"                               { return create_token(TOK_DECREMENT, yytext, yylval); }
"*"                                { return create_token(TOK_ASTERISK, yytext, yylval); }
"/"                                { return create_token(TOK_DIVIDE, yytext, yylval); }

"&"                                { return create_token(TOK_AMPERSAND, yytext, yylval); }
"|"                                { return create_token(TOK_BITWISE_OR, yytext, yylval); }
"^"                                { return create_token(TOK_BITWISE_XOR, yytext, yylval); }
"~"                                { return create_token(TOK_BITWISE_COMPL, yytext, yylval); }

"&&"                               { return create_token(TOK_LOGICAL_AND, yytext, yylval); }
"||"                               { return create_token(TOK_LOGICAL_OR, yytext, yylval); }

"=="                               { return create_token(TOK_EQUALITY, yytext, yylval); }
"!="                               { return create_token(TOK_INEQUALITY, yytext, yylval); }
"<"                                { return create_token(TOK_LT, yytext, yylval); }
"<="                               { return create_token(TOK_LTE, yytext, yylval); }
">"                                { return create_token(TOK_GT, yytext, yylval); }
">="                               { return create_token(TOK_GTE, yytext, yylval); }

"="                                { return create_token(TOK_ASSIGN, yytext, yylval); }

"if"                               { return create_token(TOK_IF, yytext, yylval); }
"else"                             { return create_token(TOK_ELSE, yytext, yylval); }
"while"                            { return create_token(TOK_WHILE, yytext, yylval); }
"for"                              { return create_token(TOK_FOR, yytext, yylval); }
"do"                               { return create_token(TOK_DO, yytext, yylval); }
"switch"                           { return create_token(TOK_SWITCH, yytext, yylval); }
"case"                             { return create_token(TOK_CASE, yytext, yylval); }
"char"                             { return create_token(TOK_CHAR, yytext, yylval); }
"short"                            { return create_token(TOK_SHORT, yytext, yylval); }
"int"                              { return create_token(TOK_INT, yytext, yylval); }
"long"                             { return create_token(TOK_LONG, yytext, yylval); }
"unsigned"                         { return create_token(TOK_UNSIGNED, yytext, yylval); }
"signed"                           { return create_token(TOK_SIGNED, yytext, yylval); }
"float"                            { return create_token(TOK_FLOAT, yytext, yylval); }
"double"                           { return create_token(TOK_DOUBLE, yytext, yylval); }
"return"                           { return create_token(TOK_RETURN, yytext, yylval); }
"break"                            { return create_token(TOK_BREAK, yytext, yylval); }
"continue"                         { return create_token(TOK_CONTINUE, yytext, yylval); }
"static"                           { return create_token(TOK_STATIC, yytext, yylval); }
"extern"                           { return create_token(TOK_EXTERN, yytext, yylval); }
"auto"                             { return create_token(TOK_AUTO, yytext, yylval); }
"const"                            { return create_token(TOK_CONST, yytext, yylval); }
"volatile"                         { return create_token(TOK_VOLATILE, yytext, yylval); }
"struct"                           { return create_token(TOK_STRUCT, yytext, yylval); }
"union"                            { return create_token(TOK_UNION, yytext, yylval); }

  /* TODO: identifier */

[ \t\r]+                           { /*cur_loc.advance(int(yyleng));*/ }
"\n"                               { /*cur_loc.next_line();*/ }

.                                  { /* TODO: report error */ }


%%

int create_token(int token_tag, const char *lexeme, YYSTYPE *semantic_value) {
  Node *tok = new Node(token_tag, lexeme);
  //tok->set_loc(cur_loc);

  semantic_value->node = tok;

  //cur_loc.advance(int(tok->get_strval().size()));

  return token_tag;
}
