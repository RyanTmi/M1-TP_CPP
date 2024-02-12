CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -std=c++11

all: TestPermutation TestSparse

TestPermutation: TestPermutation.cpp Permutation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

TestSparse: TestSparse.cpp Permutation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) TestPermutation TestSparse
