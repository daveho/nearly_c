CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -I.

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $*.cc -o $*.o

all :

lex.yy.o : lex.yy.cc parse.tab.h

# TODO: generate C++ parser
parse.tab.h parse.tab.c : parse.y
	bison -d parse.y

lex.yy.cc : lex.l
	flex lex.l

clean :
	rm -f *.o lex.yy.cc parse.tab.h parse.tab.c
