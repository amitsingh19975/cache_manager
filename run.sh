#!/bin/bash

BIN=cache
INPUT=main.cpp

clang++ $INPUT -std=c++2a -O3 $1 -o $BIN && ./$BIN && rm "./$BIN"