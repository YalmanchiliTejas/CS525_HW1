#!/bin/bash


for p in 2 4 6 8 16 32
do
    mpiexec -np $p ./question2
done