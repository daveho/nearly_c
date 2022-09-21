# NearlyC: a front-end for a substantial subset of C

This is a lexer and parser for a substantial subset of C.

C features that are (currently) missing:

* Typedefs
* Variadic functions
* Abstract declarators (e.g., omitting a parameter name from a function declaration)
* Pointers to functions
* Pointers to arrays
* Returning a pointer from a function

There are probably other things that are missing or don't work.

There are also some restrictions. For example, a storage class specifier
(`static` or `extern`) must precede the type in a declaration or definition,
so code like

```
int static x;
```

isn't allowed.

Also, in an array declaration or definition, all dimensions must have their
sizes specified as a single integer literal. For example, the following
code would not be allowed:

```c
void foo(int a[]) { // <--size of array dimension must be specified

}

int main(void) {
  int x[4*5]; // <-- size of array dimension must be a single int literal
}
```

## Why?

My intention is to use this as the source language in a project sequence
in the compilers course I teach.

Full modern C (e.g., ISO C, C99, C11, etc.) has surprising complexity.
However, with some careful omission of difficult language features, such
as typedefs, we get what I think is a core language that is powerful,
but not too difficult to understand, analyze, generate code for, etc.

## Some technical details

The lexer is implemented with Flex and the parser is implemented with Bison.

The parser is a "pure" parser, and the lexer is reentrant (and uses `%option bison-bridge`),
so no global variables are needed.

All code is C++.  Note that neither the lexer nor parser is generated as a C++
class: the "plain" code generated by both Flex and Bison compiles fine as C++.
My personal opinion is that the C++ code generation in both Flex and Bison
is confusing and has no real advantage over the standard pure parser and reentrant
scanner code generation.

A ruby script, [scan\_grammar\_symbols.rb](scan_grammar_symbols.rb),
is used to extract the names of all terminal and nonterminal symbols from
the grammar, so that they can be used to enumerate and stringify all
symbol names. This script assumes that every nonterminal symbol

* appears on a line by itself, not indented, and
* consists only of lower-case letters and underscores

## Parse trees vs. ASTs

There are actually two parsers, [parse.y](parse.y) and [parse\_buildast.y](parse_buildast.y).
The former builds a full parse tree, while the latter builds an AST
(abstract syntax tree.)  I plan to keep them in sync with each other.
The former is useful for validating the parser itself, and is probably
the better option for source to source translation, while the latter
is probably much more useful as a compiler front-end.

You can choose which parser to use by editing the [Makefile](Makefile).

## Compiling the program

Run the commands

```
make depend
make
```

## Running the program

Run the command as

```
./nearly_c -p input.c
```

(The `-p` option means "print a tree".)

Consider this code:

```c
int sum(int a, int b) {
  return a + b;
}
```

This input generates the parse tree (using [parse.y](parse.y))

```
unit
+--top_level_declaration
   +--function_or_variable_declaration_or_definition
      +--function_definition_or_declaration
         +--type
         |  +--basic_type
         |     +--basic_type_keyword
         |        +--TOK_INT[int]
         +--TOK_IDENT[sum]
         +--TOK_LPAREN[(]
         +--function_parameter_list
         |  +--opt_parameter_list
         |     +--parameter_list
         |        +--parameter
         |        |  +--type
         |        |  |  +--basic_type
         |        |  |     +--basic_type_keyword
         |        |  |        +--TOK_INT[int]
         |        |  +--declarator
         |        |     +--TOK_IDENT[a]
         |        +--TOK_COMMA[,]
         |        +--parameter_list
         |           +--parameter
         |              +--type
         |              |  +--basic_type
         |              |     +--basic_type_keyword
         |              |        +--TOK_INT[int]
         |              +--declarator
         |                 +--TOK_IDENT[b]
         +--TOK_RPAREN[)]
         +--TOK_LBRACE[{]
         +--opt_statement_list
         |  +--statement_list
         |     +--statement
         |        +--TOK_RETURN[return]
         |        +--assignment_expression
         |        |  +--conditional_expression
         |        |     +--logical_or_expression
         |        |        +--logical_and_expression
         |        |           +--bitwise_or_expression
         |        |              +--bitwise_xor_expression
         |        |                 +--bitwise_and_expression
         |        |                    +--equality_expression
         |        |                       +--relational_expression
         |        |                          +--shift_expression
         |        |                             +--additive_expression
         |        |                                +--additive_expression
         |        |                                |  +--multiplicative_expression
         |        |                                |     +--cast_expression
         |        |                                |        +--unary_expression
         |        |                                |           +--postfix_expression
         |        |                                |              +--primary_expression
         |        |                                |                 +--TOK_IDENT[a]
         |        |                                +--TOK_PLUS[+]
         |        |                                +--multiplicative_expression
         |        |                                   +--cast_expression
         |        |                                      +--unary_expression
         |        |                                         +--postfix_expression
         |        |                                            +--primary_expression
         |        |                                               +--TOK_IDENT[b]
         |        +--TOK_SEMICOLON[;]
         +--TOK_RBRACE[}]
```

This input generates the AST (using [parse\_buildast.y](parse_buildast.y))

```
AST_UNIT
+--AST_FUNCTION_DEFINITION
   +--TOK_UNSPECIFIED_STORAGE
   +--AST_BASIC_TYPE
   |  +--TOK_INT[int]
   +--TOK_IDENT[sum]
   +--AST_FUNCTION_PARAMETER_LIST
   |  +--AST_FUNCTION_PARAMETER
   |  |  +--AST_BASIC_TYPE
   |  |  |  +--TOK_INT[int]
   |  |  +--AST_NAMED_DECLARATOR
   |  |     +--TOK_IDENT[a]
   |  +--AST_FUNCTION_PARAMETER
   |     +--AST_BASIC_TYPE
   |     |  +--TOK_INT[int]
   |     +--AST_NAMED_DECLARATOR
   |        +--TOK_IDENT[b]
   +--AST_STATEMENT_LIST
      +--AST_RETURN_EXPRESSION_STATEMENT
         +--AST_BINARY_EXPRESSION
            +--TOK_PLUS[+]
            +--AST_VARIABLE_REFERENCE
            |  +--TOK_IDENT[a]
            +--AST_VARIABLE_REFERENCE
               +--TOK_IDENT[b]
```
