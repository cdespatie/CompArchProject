#include "ecse425projOPT.h"
#include "ecse425proj.h"

#include <stdio.h>
#include <stdlib.h>

#define B 	(64 / sizeof(double))		// Block size fits inside one cache line!

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

static void transposeMatrix(const double *matX, double *matY, int N);


void matVecMult_opt(int N, const double *matA, const double *vecB, double *vecC) {
	int i, j, x, y;
	double temp = 0.0;
    
    // Let's try loop blocking again!
    double *Apos1 = &matA[0];
    double *Apos2 = &matA[1];
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


    // 'Naive' attempt
    /*
	for (i = 0; i < N; i++) {
        temp = vecC[i];
        for (j = 0; j < N; j++) {
        		
        	temp += matA[j + i*N] * vecB[j];	
        }

        vecC[i] = temp;
    }
    */
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

// Transpose matrix matX
static void transposeMatrix(const double *matX, double *matY, int N) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {

			matY[j + i*N] = matX[i + j*N];

		}
	}
}
