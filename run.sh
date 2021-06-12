#!/bin/bash

BIN=cache
INPUT=main.cpp

clang++ $INPUT -std=c++2a -O3 $1 -DBOOST_NUMERIC_UBLAS_NO_MAKE_CACHE_MANAGER -o $BIN && ./$BIN && rm "./$BIN"