CXX      := g++
CFLAGS   := -Wall -Wextra -pedantic -std=c++11 -O2 -DNDEBUG

#! Please modify the following INCLUDES variable to point to the location of Eigen library on your machine
INCLUDES := /opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3

all: MatrixPower MatrixRandom

MatrixPower: MatrixPower.cpp
	$(CXX) $(CFLAGS) $^ -I $(INCLUDES) -o $@

MatrixRandom: MatrixRandom.cpp Histogram.cpp
	$(CXX) $(CFLAGS) $^ -I $(INCLUDES) -o $@

clean:
	rm -f MatrixPower MatrixRandom
