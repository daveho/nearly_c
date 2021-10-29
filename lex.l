%{

#include "node.h"
#include "parse.tab.h"

Location cur_loc;

int create_token(int tag, const char *lexeme);

typedef yy::Parser::token Token;

%}

  /* See: https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/scanner.ll */
%option c++
%option batch
%option debug
%option yywrap nounput 
%option stack

%%

"("                                { return create_token(Token::TOK_LPAREN, yytext); }
")"                                { return create_token(Token::TOK_RPAREN, yytext); }
"["                                { return create_token(Token::TOK_LBRACKET, yytext); }
"]"                                { return create_token(Token::TOK_RBRACKET, yytext); }
"{"                                { return create_token(Token::TOK_LBRACE, yytext); }
"}"                                { return create_token(Token::TOK_RBRACE, yytext); }

";"                                { return create_token(Token::TOK_SEMICOLON, yytext); }
":"                                { return create_token(Token::TOK_COLON, yytext); }

"+"                                { return create_token(Token::TOK_PLUS, yytext); }
"++"                               { return create_token(Token::TOK_INCREMENT, yytext); }
"-"                                { return create_token(Token::TOK_MINUS, yytext); }
"--"                               { return create_token(Token::TOK_DECREMENT, yytext); }
"*"                                { return create_token(Token::TOK_ASTERISK, yytext); }
"/"                                { return create_token(Token::TOK_DIVIDE, yytext); }

"&"                                { return create_token(Token::TOK_AMPERSAND, yytext); }
"|"                                { return create_token(Token::TOK_BITWISE_OR, yytext); }
"^"                                { return create_token(Token::TOK_BITWISE_XOR, yytext); }
"~"                                { return create_token(Token::TOK_BITWISE_COMPL, yytext); }

"&&"                               { return create_token(Token::TOK_LOGICAL_AND, yytext); }
"||"                               { return create_token(Token::TOK_LOGICAL_OR, yytext); }

"=="                               { return create_token(Token::TOK_EQUALITY, yytext); }
"!="                               { return create_token(Token::TOK_INEQUALITY, yytext); }
"<"                                { return create_token(Token::TOK_LT, yytext); }
"<="                               { return create_token(Token::TOK_LTE, yytext); }
">"                                { return create_token(Token::TOK_GT, yytext); }
">="                               { return create_token(Token::TOK_GTE, yytext); }

"="                                { return create_token(Token::TOK_ASSIGN, yytext); }

"if"                               { return create_token(Token::TOK_IF, yytext); }
"else"                             { return create_token(Token::TOK_ELSE, yytext); }
"while"                            { return create_token(Token::TOK_WHILE, yytext); }
"for"                              { return create_token(Token::TOK_FOR, yytext); }
"do"                               { return create_token(Token::TOK_DO, yytext); }
"switch"                           { return create_token(Token::TOK_SWITCH, yytext); }
"case"                             { return create_token(Token::TOK_CASE, yytext); }
"char"                             { return create_token(Token::TOK_CHAR, yytext); }
"short"                            { return create_token(Token::TOK_SHORT, yytext); }
"int"                              { return create_token(Token::TOK_INT, yytext); }
"long"                             { return create_token(Token::TOK_LONG, yytext); }
"unsigned"                         { return create_token(Token::TOK_UNSIGNED, yytext); }
"signed"                           { return create_token(Token::TOK_SIGNED, yytext); }
"float"                            { return create_token(Token::TOK_FLOAT, yytext); }
"double"                           { return create_token(Token::TOK_DOUBLE, yytext); }
"return"                           { return create_token(Token::TOK_RETURN, yytext); }
"break"                            { return create_token(Token::TOK_BREAK, yytext); }
"continue"                         { return create_token(Token::TOK_CONTINUE, yytext); }
"static"                           { return create_token(Token::TOK_STATIC, yytext); }
"extern"                           { return create_token(Token::TOK_EXTERN, yytext); }
"auto"                             { return create_token(Token::TOK_AUTO, yytext); }
"const"                            { return create_token(Token::TOK_CONST, yytext); }
"volatile"                         { return create_token(Token::TOK_VOLATILE, yytext); }
"struct"                           { return create_token(Token::TOK_STRUCT, yytext); }
"union"                            { return create_token(Token::TOK_UNION, yytext); }

  /* TODO: identifier */

[ \t\r]+                           { cur_loc.advance(int(yyleng)); }
"\n"                               { cur_loc.next_line(); }


%%

int create_token(int tag, const char *lexeme) {
  Node *tok = new Node(tag, lexeme);
  tok->set_loc(cur_loc);

  // How to find yylval?
  //yylval.node = tok;

  cur_loc.advance(int(tok->get_strval().size()));

  return tag;
}
