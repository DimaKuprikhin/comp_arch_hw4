#!/bin/bash
g++ -o hw4 hw4.cpp -fopenmp
printf "1 thread:\n"
time ./hw4 < inputs/input1.txt
printf "\n2 threads:\n"
time ./hw4 < inputs/input2.txt
printf "\n4 threads:\n"
time ./hw4 < inputs/input4.txt
printf "\n8 threads:\n"
time ./hw4 < inputs/input8.txt
