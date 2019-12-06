# CISC372-Dijkstra-Parallel


Contributors:
Nishant Chintala
Charlie Hannum
Tim Mazzarelli
Daniel Warszawski

This repository stores the code for a CISC372 Parallel Computing final project code. The goal of this project was to take a serial implementation fo Dijkstra's Shortest Path algorithm and parallelize the code to make it more efficient through OpenMP and OpenACC. There are 3 main folders in the repository: Serial for the serial implementation, OPENMP which contains an OpenMP parallelization, and OpenACC which has the OpenACC implementation. The OpenMP implementation is the most complete.  The graphs we used to test Dijkstra algorithm were generated randomly.  

To run the OpenMP code, compile using the command `gcc -fopenmp DijkstraOPENMP.c -lm -o DijkstraOPENMPexecutable` and execute by running `./DijkstraOPENMPexecutable`. The output time displayed after running is the time it took to complete the algorithm. The time included for the adjacency matrix creation is not included in this time.

To run the OpenMP code, compile using the command `pgcc -fast Dijkstra-OpenACC.c -lm -o DijkstraOPENACCexecutable` and execute by running `./DijkstraOPENACCexecutable`. The output time displayed after running is the time it took to complete the algorithm. The time included for the adjacency matrix creation is not included in this time.

Serial code original source: http://people.math.sc.edu/Burkardt/c_src/dijkstra/dijkstra.c

Parallel code original source: http://people.math.sc.edu/Burkardt/c_src/dijkstra_openmp/dijkstra_openmp.c
