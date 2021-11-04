CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -I.

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all :

parse.tab.h parse.tab.cpp : parse.y
	bison --output-file=parse.tab.cpp --defines=parse.tab.h parse.y

lex.yy.cc : lex.l
	flex lex.l

clean :
	rm -f *.o parse.tab.h parse.tab.cpp
