#!/bin/bash

# Compile the code
mpic++ q3.cpp -o run

# Loop through processors (2, 4, 8, 16, 32) and list sizes (100, 1000, 10000)
for p in 2 4 8 16 32; do
  for ns in 100 1000 10000; do
    echo "mpirun -np $p ./run $ns"
    mpirun -np $p ./run $ns
  done
done
