#!/bin/bash
# number of threads:


for t in 1 2 4 6 8 10
do
  echo NUMT = $t
  # number of subdivisions:
  for s in 1 10 100 500 1000 5000 10000 50000 100000
  do
    echo NUMTRIALS = $s
    g++ -DNUMT=$t -DNUMTRIALS=$s prog.cpp -o prog -lm -fopenmp
    ./prog &>> myfile.txt
  done
done