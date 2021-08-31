#!/bin/bash

#SBATCH -J MonteCarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o openCL.out
#SBATCH -e openCL.err
#SBATCH --mail-type=END
#SBATCH --mail-user=liusus@oregonstate.edu


for b in 16 32 64 128 256
do
  for k in 2 4 8 16
  do
    g++ /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -DLOCAL_SIZE=$b -DNMB=$k -o first first.cpp -lm -fopenmp;
    ./first&>>firstdgx.txt
    g++ /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -DLOCAL_SIZE=$b -DNMB=$k -o second second.cpp -lm -fopenmp;
    ./second&>>seconddgx.txt
      g++ /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -DLOCAL_SIZE=$b -DNMB=$k -o third third.cpp -lm -fopenmp;
    ./third&>>thirddgx.txt
  done
done
