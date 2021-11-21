CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -I.

GENERATED_SRCS = parse.tab.cpp lex.yy.cpp grammar_symbols.cpp
SRCS = node.cpp location.cpp treeprint.cpp \
	main.cpp ast.cpp arena.cpp \
	yyerror.cpp exceptions.cpp cpputil.cpp $(GENERATED_SRCS)
OBJS = $(SRCS:%.cpp=%.o)

# Uncomment one of the following depending on whether you
# want the parser to build a parse tree or build an AST
#PARSER_SRC = parse.y
PARSER_SRC = parse_buildast.y

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all : nearly_c

nearly_c : parse.tab.h lex.yy.h grammar_symbols.h grammar_symbols.cpp $(OBJS)
	$(CXX) -o $@ $(OBJS)

parse.tab.h parse.tab.cpp : $(PARSER_SRC)
	bison -v --output-file=parse.tab.cpp --defines=parse.tab.h $(PARSER_SRC)

lex.yy.cpp lex.yy.h : lex.l
	flex --outfile=lex.yy.cpp --header-file=lex.yy.h lex.l

grammar_symbols.h grammar_symbols.cpp : $(PARSER_SRC)
	./scan_grammar_symbols.rb < $(PARSER_SRC)

depend : parse.tab.h lex.yy.h grammar_symbols.h
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

clean :
	rm -f *.o depend.mak \
		$(GENERATED_SRCS) $(GENERATED_SRCS:%.cpp=%.h) \
		-f parse.output nearly_c

include depend.mak
