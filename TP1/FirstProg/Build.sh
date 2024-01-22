#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -pedantic -std=c++11"

clang++ $CFLAGS premierprog.cpp -o premierprog
clang++ $CFLAGS secondprog.cpp -o secondprog
