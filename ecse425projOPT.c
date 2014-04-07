#include "ecse425projOPT.h"
#include "ecse425proj.h"

#include <stdio.h>
#include <stdlib.h>

#define B 	(64 / sizeof(double))

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

static void transposeMatrix(const double *matX, double *matY, int N);


void matVecMult_opt(int N, const double *matA, const double *vecB, double *vecC) {
    
    // Code in your optimized implementation here!
}

void matMult_opt(int N, const double *matA, const double *matB, double *matC) {
   
    int j,i,m;
    int x,y,z;
    double temp = 0.0;
    double *matY = malloc(N*N*sizeof(double));

    // Transpose matB for sequential memory access during multiply
   	transposeMatrix(matB, matY, N);

   	// Old matrix multiplication w\o loop tiling
   	/*
    for (i = 0; i < N; i++) {
    	for (j = 0; j < N; j++) {
    		for (m = 0; m < N; m++) {

    			temp += matA[i*N + m] * matY[j*N + m];

    		}

    		matC[j + i*N] = temp;
    		temp = 0.0;
    	}
    }
    */

    // New matrix multiplication w\ loop tiling
    
    for (i = 0; i < N; i += B) {
    	for (j = 0; j < N; j += B) {
    		for (m = 0; m < N; m+= B) {

    			for (x = i; x < MIN(i + B, N); x++) {
    				for (y = j; y < MIN(j + B, N); y++) {
    					for (z = m; z < MIN(m + B, N); z++) {

    						temp += matA[x*N + z] * matY[y*N + z];

    					}

    					matC[y + x*N] = temp;
    					temp = 0.0;
    				}
    			}
    		}
    	}
    }
    

}

// Transpose matrix matX in place [O(1) space requirement]
static void transposeMatrix(const double *matX, double *matY, int N) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {

			matY[j + i*N] = matX[i + j*N];

		}
	}
}
