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

#include "node.h"
#include "parse.tab.h"
#include "parser_state.h"

int create_token(int, const char *, YYSTYPE *, ParserState *);

// Macro to get the pointer to the ParserState from the lexer
// state, which is available (according to YY_DECL) in the
// "yyscanner" parameter to yylex()
#define PSTATE() static_cast<ParserState *>(yyget_extra(yyscanner))

// Macro to create a token and return its tag value.
// Avoids quite a bit of code duplication in the scanner rules.
#define CRTOK(tag) return create_token(tag, yytext, yylval, PSTATE())

// Declaration of yyerror()
void yyerror(struct ParserState *, const char *);
%}

%option noyywrap nounput reentrant bison-bridge

%x C_COMMENT

%%

"("                        { CRTOK(TOK_LPAREN); }
")"                        { CRTOK(TOK_RPAREN); }
"["                        { CRTOK(TOK_LBRACKET); }
"]"                        { CRTOK(TOK_RBRACKET); }
"{"                        { CRTOK(TOK_LBRACE); }
"}"                        { CRTOK(TOK_RBRACE); }

";"                        { CRTOK(TOK_SEMICOLON); }
":"                        { CRTOK(TOK_COLON); }
","                        { CRTOK(TOK_COMMA); }
"."                        { CRTOK(TOK_DOT); }
"?"                        { CRTOK(TOK_QUESTION); }
"!"                        { CRTOK(TOK_NOT); }

"+"                        { CRTOK(TOK_PLUS); }
"++"                       { CRTOK(TOK_INCREMENT); }
"-"                        { CRTOK(TOK_MINUS); }
"--"                       { CRTOK(TOK_DECREMENT); }
"*"                        { CRTOK(TOK_ASTERISK); }
"/"                        { CRTOK(TOK_DIVIDE); }
"%"                        { CRTOK(TOK_MOD); }

"&"                        { CRTOK(TOK_AMPERSAND); }
"|"                        { CRTOK(TOK_BITWISE_OR); }
"^"                        { CRTOK(TOK_BITWISE_XOR); }
"~"                        { CRTOK(TOK_BITWISE_COMPL); }
"<<"                       { CRTOK(TOK_LEFT_SHIFT); }
">>"                       { CRTOK(TOK_RIGHT_SHIFT); }

"&&"                       { CRTOK(TOK_LOGICAL_AND); }
"||"                       { CRTOK(TOK_LOGICAL_OR); }

"=="                       { CRTOK(TOK_EQUALITY); }
"!="                       { CRTOK(TOK_INEQUALITY); }
"<"                        { CRTOK(TOK_LT); }
"<="                       { CRTOK(TOK_LTE); }
">"                        { CRTOK(TOK_GT); }
">="                       { CRTOK(TOK_GTE); }

"="                        { CRTOK(TOK_ASSIGN); }
"*="                       { CRTOK(TOK_MUL_ASSIGN); }
"/="                       { CRTOK(TOK_DIV_ASSIGN); }
"%="                       { CRTOK(TOK_MOD_ASSIGN); }
"+="                       { CRTOK(TOK_ADD_ASSIGN); }
"-="                       { CRTOK(TOK_SUB_ASSIGN); }
"<<="                      { CRTOK(TOK_LEFT_ASSIGN); }
">>="                      { CRTOK(TOK_RIGHT_ASSIGN); }
"&="                       { CRTOK(TOK_AND_ASSIGN); }
"^="                       { CRTOK(TOK_XOR_ASSIGN); }
"|="                       { CRTOK(TOK_OR_ASSIGN); }

"if"                       { CRTOK(TOK_IF); }
"else"                     { CRTOK(TOK_ELSE); }
"while"                    { CRTOK(TOK_WHILE); }
"for"                      { CRTOK(TOK_FOR); }
"do"                       { CRTOK(TOK_DO); }
"switch"                   { CRTOK(TOK_SWITCH); }
"case"                     { CRTOK(TOK_CASE); }
"char"                     { CRTOK(TOK_CHAR); }
"short"                    { CRTOK(TOK_SHORT); }
"int"                      { CRTOK(TOK_INT); }
"long"                     { CRTOK(TOK_LONG); }
"unsigned"                 { CRTOK(TOK_UNSIGNED); }
"signed"                   { CRTOK(TOK_SIGNED); }
"float"                    { CRTOK(TOK_FLOAT); }
"double"                   { CRTOK(TOK_DOUBLE); }
"void"                     { CRTOK(TOK_VOID); }
"return"                   { CRTOK(TOK_RETURN); }
"break"                    { CRTOK(TOK_BREAK); }
"continue"                 { CRTOK(TOK_CONTINUE); }
"static"                   { CRTOK(TOK_STATIC); }
"extern"                   { CRTOK(TOK_EXTERN); }
"auto"                     { CRTOK(TOK_AUTO); }
"const"                    { CRTOK(TOK_CONST); }
"volatile"                 { CRTOK(TOK_VOLATILE); }
"struct"                   { CRTOK(TOK_STRUCT); }
"union"                    { CRTOK(TOK_UNION); }

[A-Za-z_][A-Za-z_0-9]*     { CRTOK(TOK_IDENT); }

  /*
   * String and character literals.
   * Probably not 100% accurate, but hopefully close enough.
   */
\"([^\\\"]|\\.)*\"         { CRTOK(TOK_STR_LIT); }
'(\\.|[^\\'\n])'           { CRTOK(TOK_CHAR_LIT); }

  /*
   * Numeric literals: these probably aren't exactly right, but
   * should be a close approximation.
   */
[0-9]+                     { CRTOK(TOK_INT_LIT); }
0[Xx][0-9A-Fa-f]+[ULul]*   { CRTOK(TOK_INT_LIT); }
[0-9]+\.[0-9]*[Ff]?        { CRTOK(TOK_FP_LIT); }


[ \t\r]+                   { PSTATE()->cur_loc.advance(int(yyleng)); }
\n                         { PSTATE()->cur_loc.next_line(); }

  /*
   * C-style block comment
   * See: https://stackoverflow.com/questions/2130097/
   */
"/*"               { BEGIN(C_COMMENT); PSTATE()->cur_loc.advance(int(yyleng)); }
<C_COMMENT>"*/"    { BEGIN(INITIAL); PSTATE()->cur_loc.advance(int(yyleng)); }
<C_COMMENT>\n      { PSTATE()->cur_loc.next_line(); }
<C_COMMENT>.       { PSTATE()->cur_loc.advance(int(yyleng)); }

  /*
   * C++-style comment
   */
"//"[^\n]*\n       { PSTATE()->cur_loc.next_line(); }

.                  { yyerror(PSTATE(), "Unrecognized character"); }


%%

int create_token(int token_tag, const char *lexeme, YYSTYPE *semantic_value, ParserState *pp) {
  Node *tok = new Node(token_tag, lexeme);
  tok->set_loc(pp->cur_loc);

  semantic_value->node = tok;

  pp->cur_loc.advance(int(tok->get_strval().size()));

  //printf("read token: %s(%d)\n", lexeme, token_tag);

  return token_tag;
}
