FLEX=/usr/local/opt/flex/bin/flex
BISON=/usr/local/opt/bison/bin/bison

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(DBGFLAGS) -std=c++2b -MD
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=Parser.cpp Scanner.cpp main.cpp astnode.cpp QifObjects.cpp date-parsers.cpp
OBJS:=$(addprefix build/, $(SOURCES:.cpp=.o))
DEPS:=$(OBJS:.o=.d)

parser: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

build/%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

build/%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

Scanner.cpp: qif.l Parser.hpp Scanner.hpp
	$(FLEX) $<

Parser.cpp Parser.hpp: qif.y
	$(BISON) -Wcounterexamples -d $^

.PHONY: clean

clean:
	rm -f Parser.cpp Scanner.cpp Parser.hpp parser location.hh
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)
	rm -f $(DEPS)

.PHONY:	profile

profile:	parser
	/usr/local/opt/llvm/bin/llvm-profdata merge -output=merge.out -instr default.profraw

-include $(DEPS)