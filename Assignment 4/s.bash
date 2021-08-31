#!/bin/bash
  g++  -c  simd.asm.cpp -o simd.p5.o
  
  for s in 1000 10000 100000 500000 1000000 2000000 4000000 8000000
  do
    g++  -o arraymult -DSize=$s prog.cpp simd.p5.o  -lm  -fopenmp
  	./arraymult&>> num.csv
  done