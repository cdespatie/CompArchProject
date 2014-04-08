#include "ecse425projOPT.h"
#include "ecse425proj.h"

#include <stdio.h>
#include <stdlib.h>

#define B 	(64 / sizeof(double))		// Block size fits inside one cache line!

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

static void transposeMatrix(const double *matX, double *matY, int N);
static void transposeAndPadMatrix(const double *matX, double *matY, int N);
static void padMatrix(const double *matX, double *matY, int N);
static int isPowerOfTwo (unsigned int x);


void matVecMult_opt(int N, const double *matA, const double *vecB, double *vecC) {
	int i, j, x, y;
	double temp = 0.0;
    
    // Let's try loop blocking again!
    // This implementation is inspired by the page at:
    // http://simulationcorner.net/index.php?page=fastmatrixvector
    // By Sebastian Macke

    double *Apos1 = &matA[0];
    double *Apos2 = &matA[N];
    double *ypos  = &vecC[0];

    for (i = 0; i < N / 2; i ++) {
        double ytemp1 = 0;
        double ytemp2 = 0;
        double *xpos = &vecB[0];

        for (j = 0; j < N; j++) {

            ytemp1 += (*Apos1++) * (*xpos);
            ytemp2 += (*Apos2++) * (*xpos);

            xpos++;

        }

        *ypos = ytemp1;
        ypos++;
        *ypos = ytemp2;

        ypos++;

        Apos1 += N;
        Apos2 += N;
    }

}

void matMult_opt(int N, const double *matA, const double *matB, double *matC) {
   
    int j,i,m;
    int x,y,z;
    double temp = 0.0;
    double *matY;
    double *matH;
    int newDim;

    // Transpose matB for sequential memory access during multiply
   	transposeMatrix(matB, matY, N);

    if (isPowerOfTwo(N)) {

        matY = (double *) malloc((N+1)*(N+1)*sizeof(double));
        matH = (double *) malloc((N+1)*(N+1)*sizeof(double));
        padMatrix(matA, matH, N);
        transposeAndPadMatrix(matB, matY, N);

        newDim = N + 1;

        for (i = 0; i < newDim; i += B) {
            for (j = 0; j < newDim; j += B) {
                for (m = 0; m < newDim; m+= B) {

                    for (x = i; x < MIN(i + B, newDim); x++) {
                        for (y = j; y < MIN(j + B, newDim); y++) {

                            if (y != N && x != N) temp = matC[y + x*N];
                            for (z = m; z < MIN(m + B, newDim); z++) {

                                temp += matA[x*N + z] * matY[y*N + z];

                            }

                            if (y != N && x != N) matC[y + x*N] = temp;
                        }
                    }
                }
            }
        }

    } else {
        matY = (double *) malloc(N*N*sizeof(double));
        transposeMatrix(matB, matY, N);

        for (i = 0; i < N; i += B) {
            for (j = 0; j < N; j += B) {
                for (m = 0; m < N; m+= B) {

                    for (x = i; x < MIN(i + B, N); x++) {
                        for (y = j; y < MIN(j + B, N); y++) {

                            temp = matC[y + x*N];
                            for (z = m; z < MIN(m + B, N); z++) {

                                temp += matA[x*N + z] * matY[y*N + z];

                            }

                            matC[y + x*N] = temp;
                        }
                    }
                }
            }
        }
    }

    // New matrix multiplication w\ loop tiling
    
    

}

// Transpose matrix matX
static void transposeMatrix(const double *matX, double *matY, int N) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {

			matY[j + i*N] = matX[i + j*N];

		}
	}
}

// Pads matrix X with a column and row of zeroes to avoid cache superalignment on powers of two
// Also transposes!
static void transposeAndPadMatrix(const double *matX, double *matY, int N) {

    int newDimensions;
    int i, j;

    newDimensions = N + 1;


    for (i = 0; i < newDimensions; i++) {
        for (j = 0; j < newDimensions; j++) {

            if (j == newDimensions - 1 || i == newDimensions - 1) {
                matY[j + i*N] = 0;
            }
            else {
                matY[j + i*N] = matX[i + j*N];
            }

        }
    }

}

static void padMatrix(const double *matX, double *matY, int N) {

    int i, j;
    int newDimensions;

    newDimensions = N + 1;

    for (i = 0; i < newDimensions; i++) {
        for (j = 0; j < newDimensions; j++) {

            if (j == newDimensions - 1 || i == newDimensions - 1) {
                matY[j + i*N] = 0;
            }
            else {
                matY[j + i*N] = matX[j + i*N];
            }

        }
    }

}

// Checks if number is a power of two via complement and compare
// Source: http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
// By Rick Regan
static int isPowerOfTwo (unsigned int x) {
  return ((x != 0) && ((x & (~x + 1)) == x));
}