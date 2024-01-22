#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -pedantic -std=c++20"

clang++ $CFLAGS Main.cpp Analysis.cpp -o Analysis
