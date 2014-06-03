/*
 * BlasTest.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: kjopek
 */

#include <cstdio>
#include <cmath>
using namespace std;

extern "C" {
	#include "vecLib/clapack.h"
	#include "vecLib/cblas.h"
}

int main()
{

	int n = 4;
	int m = 2;
	int k = 2;
	int ipiv[m];

	double ** matrix;
	double *rhs;

	double data[4][4] = {
			{1, 2, 0, 0},
			{0, 1, 0, 0},
			{0,0, 1, 1},
			{01, 0, 0,1}
	};

	double data_r[4] = {21,10,100,1000};

	matrix = new double*[n];
	matrix[0] = new double[n*(n+1)];
	matrix[1] = matrix[0] + n;
	matrix[2] = matrix[0] + 2*n;
	matrix[3] = matrix[0] + 3*n;
	rhs = matrix[0] + 4*n;

	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j) {
			matrix[i][j] = data[i][j];
		}
		rhs[i] = data_r[i];
	}

    int status;
    // 1
	dgetrf_(&m, // size
			&m,
			matrix[0], // pointer to data
			&n, // LDA = matrix_size
			ipiv, &status); // pivot vector

    // 2.1
	cblas_dtrsm(CblasRowMajor,
			CblasLeft,
			CblasLower,
			CblasNoTrans,
			CblasUnit,
			m,
			k,
			1.0,
			matrix[0],
			n,
			matrix[0]+m,
			n);

	printf("%d %d -> %f\n", 0, 2, fabs(matrix[0][2]-0));
	printf("%d %d -> %f\n", 0, 3, fabs(matrix[0][3]-3));
	printf("%d %d -> %f\n", 1, 2, fabs(matrix[1][2]+1));
	printf("%d %d -> %f\n", 1, 3, fabs(matrix[1][3]+5));

    // 2.2
	cblas_dtrsm(CblasRowMajor,
			CblasLeft,
			CblasUpper,
			CblasNoTrans,
			CblasUnit,
			m,
			k,
			1.0,
			matrix[0],
			n,
			matrix[0]+m,
			n);

	printf("%d %d -> %f\n", 0, 2, fabs(matrix[0][2]-1));
	printf("%d %d -> %f\n", 0, 3, fabs(matrix[0][3]-8));
	printf("%d %d -> %f\n", 1, 2, fabs(matrix[1][2]+1));
	printf("%d %d -> %f\n", 1, 3, fabs(matrix[1][3]+5));

    // 3.1
	cblas_dtrsm(CblasRowMajor,
			CblasLeft,
			CblasLower,
			CblasNoTrans,
			CblasUnit,
			m,
			1,
			1.0,
			matrix[0],
			n,
			rhs,
			1);

	printf("%d -> %f\n", 0, fabs(rhs[0]-4));
	printf("%d -> %f\n", 1, fabs(rhs[1]+7));

    // 3.2
	cblas_dtrsm(CblasRowMajor,
			CblasLeft,
			CblasUpper,
			CblasNoTrans,
			CblasUnit,
			m,
			1,
			1.0,
			matrix[0],
			n,
			rhs,
			1);

	printf("%d -> %f\n", 0, fabs(rhs[0]-11));
	printf("%d -> %f\n", 1, fabs(rhs[1]+7));

    // 4.1 
	cblas_dgemm(CblasRowMajor,
			CblasNoTrans,
			CblasNoTrans,
			k,
			k,
			m,
			-1.0,
			matrix[m],
			n,
			matrix[0]+m,
			n,
			1.0,
			matrix[m]+m,
			n);

	printf("%d %d -> %f\n", 2, 2, fabs(matrix[2][2]+5));
	printf("%d %d -> %f\n", 2, 3, fabs(matrix[2][3]+27));
	printf("%d %d -> %f\n", 3, 2, fabs(matrix[3][2]-3));
	printf("%d %d -> %f\n", 3, 3, fabs(matrix[3][3]+7));


	cblas_dgemv(CblasRowMajor,
			CblasNoTrans,
			k,
			m,
			-1.0,
			matrix[m],
			n,
			rhs,
			1,
			1.0,
			rhs+m,
			1);

	printf("%d -> %f\n", 2, fabs(rhs[2]+43));
	printf("%d -> %f\n", 3, fabs(rhs[3]+4));
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++) 
            printf("%f  \t", matrix[i][j]);
        printf(" | %f  ", rhs[i]);
        printf("\n");
    }

	return 0;
}

