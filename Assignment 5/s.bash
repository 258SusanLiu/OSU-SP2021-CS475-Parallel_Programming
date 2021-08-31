#!/bin/bash

#SBATCH -J MonteCarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=END
#SBATCH --mail-user=liusus@oregonstate.edu

for b in 16 32 64 128 256
do
  for k in 2 4 8 16 32 64 128 256 512 1000
  do
    n=$((k*1024));
    /usr/local/apps/cuda/cuda-10.1/bin/nvcc -o montecarlo -DBLOCKSIZE=$b -DNUMTRIALS=$n montecarlo.cu;
    ./montecarlo&>> result.csv
  done
done