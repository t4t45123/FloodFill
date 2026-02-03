#!/bin/ksh

g++ genmaze.cpp -o maze     -lsfml-graphics -lsfml-window -lsfml-system     -std=c++17 -O2

./maze
