#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
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
	#pragma omp parallel for num_threads(5) shared(matrix,resMatrix,dataSize,buff,dimension) private (i,j,k) reduction(+: resMatrix) schedule(static)
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
	}
}