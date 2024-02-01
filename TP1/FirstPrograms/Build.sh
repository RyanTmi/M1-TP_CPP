#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -pedantic -std=c++11"

clang++ $CFLAGS FirstProgram.cpp -o FirstProgram
clang++ $CFLAGS SecondProgram.cpp -o SecondProgram
