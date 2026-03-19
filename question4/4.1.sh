#!/bin/bash


for p in 1 2 3 4 5 6 7 8
do
    mpiexec -np $p ./4.1
done