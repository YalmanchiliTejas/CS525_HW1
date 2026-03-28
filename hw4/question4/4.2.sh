#!/bin/bash


for p in  1 8
do
    mpiexec -np $p ./4.2
done