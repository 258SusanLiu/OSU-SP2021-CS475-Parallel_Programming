#!/bin/bash
# number of threads:


for t in 1 2 4
do
  echo NUMT = $t
  # number of subdivisions:
  for s in 10 20 40 80 160 320 640 1000
  do
    g++ -DNUMT=$t -DNUMNODES=$s prog.cpp -o prog -lm -fopenmp
    ./prog &>> myfile.txt
  done
done