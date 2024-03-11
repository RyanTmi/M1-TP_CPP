CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -std=c++11

all: Analysis

Analysis: TextAnalysis.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) Analysis
