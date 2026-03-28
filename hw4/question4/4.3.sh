#!/bin/bash



for n in 512 1024 1536 2048 2560 3072 3584 4096
do
    mpiexec -np 1 ./4.3 $n
done
for p in 1 2 3 4 5 6 7 8
do
    mpiexec -np $p ./4.3
done