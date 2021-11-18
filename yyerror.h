#ifndef YYERROR_H
#define YYERROR_H

#ifdef __GNUC__
#  define YYERROR_PRINTF_FORMAT __attribute__ ((format (printf, 2, 3)))
#else
#  define YYERROR_PRINTF_FORMAT
#endif

// Declaration of our yyerror() implementation
void yyerror(struct ParserState *, const char *, ...) YYERROR_PRINTF_FORMAT;

#endif // YYERROR_H
