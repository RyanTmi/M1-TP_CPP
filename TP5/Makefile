CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -std=c++11

all: MonteCarlo

MonteCarlo: Simulation.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) MonteCarlo
