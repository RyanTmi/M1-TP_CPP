CXX    := g++
CFLAGS := -Wall -Wextra -pedantic -std=c++11

all: FirstProgram SecondProgram

FirstProgram: FirstProgram.cpp
	$(CXX) $(CFLAGS) $^ -o $@

SecondProgram: SecondProgram.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) FirstProgram SecondProgram
