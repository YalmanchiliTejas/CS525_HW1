#!/bin/bash


for p in  8
do
    mpiexec -np $p ./4.2
done