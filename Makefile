CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -I.

GENERATED_SRCS = parse.tab.cpp lex.yy.cpp grammar_symbols.cpp
SRCS = node.cpp location.cpp treeprint.cpp main.cpp ast.cpp $(GENERATED_SRCS)
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all : nearly_c

nearly_c : parse.tab.h lex.yy.h grammar_symbols.h grammar_symbols.cpp $(OBJS)
	$(CXX) -o $@ $(OBJS)

parse.tab.h parse.tab.cpp : parse.y
	bison -v --output-file=parse.tab.cpp --defines=parse.tab.h parse.y

lex.yy.cpp lex.yy.h : lex.l
	flex --outfile=lex.yy.cpp --header-file=lex.yy.h lex.l

grammar_symbols.h grammar_symbols.cpp : parse.y
	./scan_grammar_symbols.rb < parse.y

depend : parse.tab.h lex.yy.h grammar_symbols.h
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

clean :
	rm -f *.o depend.mak
	rm -f $(GENERATED_SRCS) $(GENERATED_SRCS:%.cpp=%.h)

include depend.mak
