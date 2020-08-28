#!/bin/bash
g++ -O2 -std=c++17 grader.cpp ramen.cpp -o grader

for f in in/*.txt; do echo -n "[$f] "; ./grader < $f; done