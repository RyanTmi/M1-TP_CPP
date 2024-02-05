#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -pedantic -DNDEBUG -O2 -std=c++20"
LIBS=""

INCLUDES="/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3"

# clang++ $CFLAGS Matrix.cpp -o Matrix -I $INCLUDES
clang++ $CFLAGS MatrixRandom.cpp Histogram.cpp -o MatrixRandom -I $INCLUDES
