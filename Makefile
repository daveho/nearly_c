CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -I.

GENERATED_SRCS = parse.tab.cpp lex.yy.cpp
SRCS = node.cpp location.cpp main.cpp $(GENERATED_SRCS)
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all : nearly_c

nearly_c : $(OBJS) parse.tab.h lex.yy.h
	$(CXX) -o $@ $(OBJS)

parse.tab.h parse.tab.cpp : parse.y
	bison --output-file=parse.tab.cpp --defines=parse.tab.h parse.y

lex.yy.cpp lex.yy.h : lex.l
	flex --outfile=lex.yy.cpp --header-file=lex.yy.h lex.l

depend : parse.tab.h lex.yy.h
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

clean :
	rm -f *.o parse.tab.h parse.tab.cpp depend.mak

include depend.mak
