#!/bin/bash
  g++ -o prog prog.cpp -lm -fopenmp
  ./prog >> out.csv; 