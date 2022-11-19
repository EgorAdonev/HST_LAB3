#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//int gcd(int a, int b) {
//return( b != 0 ? gcd(b, a % b) : a); }
int** matriceMult(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Enter filename1 and filename2 [filename1] [filename2]");
        exit(0);
    }
    FILE *fIn;
    fIn = fopen(argv[1],"r");
	
	FILE *fOut;
    fOut = fopen(argv[2],"w+");
	
    fIn ? printf("file1 opened \n") : exit(0);
	fOut ? printf("file2 opened \n") : exit(0);
    printf("INFO: Files %s,%s in mode %s successfully opened\n", argv[1], argv[2], "r");

	double startTime = clock();

    int** matrix;
	matrix = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        matrix[i] = (int*) malloc(3 * sizeof(int));
    }
    int** resMatrix;
	resMatrix = (int**) malloc(3 * sizeof(int*));
	for (int i = 0; i < 3; i++) {
        resMatrix[i] = (int*) malloc(3 * sizeof(int));
    }
	char buff[16], numberChar[2];
	int dataSize = 0;
	while(1){
		int dimension = 1;
		for (int j = 0; j < dimension; j++){
			int size = fscanf(fIn, "%[^\n]%*с%[0-9 ]", buff);
			if (size != EOF){
				int i = 0;
				int number = 0, localDimension = 0;
				while(buff[i] != NULL){
					if(buff[i] != ' ') {
						numberChar[0] = buff[i];
						number *= 10;
						number += atoi(numberChar);
					}
					else{
						matrix[j][localDimension] = number;
						if (j == 0){
							dimension++;
						}
						localDimension ++;
						number = 0;
					}
					i++;
				}
				if (j == 0){
					dimension--;
				}
			}
			else{
				fprintf(fOut, "Count time: %f ms\n", clock() - startTime);
				fprintf(fOut, "Size of data: %f Mb\n", ((double) dataSize) / (1024.0 * 1024.0));
				fclose(fOut);
				exit(0);
			}
		}
		for(int i = 0; i < dimension;i++){
			for(int j = 0; j < dimension; j++){
				resMatrix[i][j] = 0;
				for(int k = 0; k < dimension; k++){
					resMatrix[i][j] += matrix[i][k]*matrix[k][j];
				}
				fprintf(fOut, "%d ", resMatrix[i][j]); 
			}
			fprintf(fOut, "\n"); 
		}
			dataSize += dimension*dimension*4;
			return fOut;
	}
}
int main(int argc, char **argv) {
		FILE *outputFile;
		int numtasks, rank, rc, i;
		int *sbuf, rbuf[2], result;
		if((rc = MPI_Init(&argc, &argv)) != MPI_SUCCESS) {
			fprintf(stderr, "MPI programme error \n");
			MPI_Abort(MPI_COMM_WORLD, rc);
		}
		MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if(rank == 0) {
			printf("num of tasks = %d\n", numtasks);
			sbuf = (int *)malloc(numtasks * sizeof(int) * 2);
			srand(time(NULL));
			for(i=0; i < numtasks * 2; i++) {
				sbuf[i] = rand() % 10000;
			}
			MPI_Scatter(sbuf, 2, MPI_INT, rbuf, 2, MPI_INT, 0, MPI_COMM_WORLD);
			outputFile = matriceMult(int argc, char *argv[]);
			//gcdv = gcd(rbuf[0], rbuf[1]);
			printf("current rank=%d r1=%d r2=%d result=%d\n", rank, rbuf[0], rbuf[1], gcdv);
			MPI_Reduce(&gcdv, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			if(rank == 0) {
				printf("result is %d\n", result);
			}
			MPI_Finalize();
			return 0;
		}
}