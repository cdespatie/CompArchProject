#include "ecse425projOPT.h"
#include "ecse425proj.h"


void matVecMult_opt(int N, const double *matA, const double *vecB, double *vecC) {
    
    // Code in your optimized implementation here!
}

void matMult_opt(int N, const double *matA, const double *matB, double *matC) {
   
    int j,i,m;
    double temp = 0.0;

    // Transpose matB for sequential memory access during multiply
    transposeMatrix(matB, N);

    for (i = 0; i < N; i++) {
    	for (j = 0; j < N; j++) {
    		for (m = 0; m < N; m++) {

    			temp += matA[i*N + m] * matB[j*N + m];

    		}

    		matC[j + i*N] = temp;
    		temp = 0.0;
    	}
    }

}

// Transpose matrix matX in place [O(1) space requirement]
static void transposeMatrix(const double *matX, int N) {
	int i, j;
	double temp;

	for (i = 0; i < N - 2; i++) {
		for (j = i + 1; j < N - 1; j++) {

			// Swap matX(i,j) and matX(j,i)
			temp = matX[i*N + j];
			matX[i*N + j] = matX[j*N + i];
			matX[j*N + i] = temp;
		}
	}

}