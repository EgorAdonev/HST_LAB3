#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int gcd(int a, int b) {
return( b != 0 ? gcd(b, a % b) : a); }
int main(int argc, char **argv) {

int numtasks, rank, rc, i, gcdv;
int *sbuf, rbuf[2], result;
if((rc = MPI_Init(&argc, &argv)) != MPI_SUCCESS) {

fprintf(stderr, "Error starting MPI program. Terminating.\n");
MPI_Abort(MPI_COMM_WORLD, rc);

}
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
if(rank == 0) {

printf("Number of tasks=%d\n", numtasks);
sbuf = (int *)malloc(numtasks * sizeof(int) * 2);
srand(time(NULL));
for(i=0; i < numtasks * 2; i++) sbuf[i] = rand() % 10000;

}