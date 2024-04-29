CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -O2 -std=c++11

all: test_urne

test_urne: test_urne.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_urne
