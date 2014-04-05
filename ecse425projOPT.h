#ifndef ECSE425PROJOPT_H
#define ECSE425PROJOPT_H

void matMult_opt(int N, const double *matA, const double *matB, double *matC);

void matVecMult_opt(int N, const double *matA, const double *vecB, double *vecC);

static void transposeMatrix(const double *matX, int N);

#endif 

