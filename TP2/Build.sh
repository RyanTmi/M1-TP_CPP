#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -pedantic -O2 -std=c++20"
LIBS=""

INCLUDES="/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3"

FILES="Matrix.cpp"
EXEC="Matrix"

clang++ $CFLAGS $FILES -o $EXEC -I $INCLUDES
