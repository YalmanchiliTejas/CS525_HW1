#!/bin/bash

for W in 1 4 16 64
do
    echo "Running with p=8 and W=$W"
    mpiexec -np 8 ./4.4 $W
done