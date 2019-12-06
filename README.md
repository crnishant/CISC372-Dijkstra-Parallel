# CISC372-Djikstra-Parallel


Contributors:
Nishant Chintala
Charlie Hannum
Tim Mazzarelli
Daniel Warszawski

This repository stores the code for a CISC372 Parallel Computing final project code. The goal of this project was to take a serial implementation fo Dijkstra's Shortest Path algorithm and parallelize the code to make it more efficient through OpenMP and OpenACC. There are 3 main folders in the repository: Serial for the serial implementation, Parallel which contains an OpenMP parallelization, and OpenACC which has the OpenACC implementation. The OpenMP implementation is the most complete.

To run the OpenMP code, compile using the command `gcc -fopenmp Parallel-Dijkstra.c -lm` and execute by running `./a.out`
