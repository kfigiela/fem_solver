#include "merge.h"
#include <cstdio>
#include <iostream>
#include <vector>
extern "C" {
	#include "vecLib/clapack.h"
	#include "vecLib/cblas.h"
}

void eliminate(double * matrix, double * rhs, int n, int m) {

    int status;
	int ipiv[m];
	int k = n - m;
	
    // 1
	dgetrf_(
			/* M    */ &m, // size
			/* N    */ &m,
			/* A    */ matrix, // pointer to data
			/* LDA  */ &n, // LDA = matrix_size
			/* IPIV */ ipiv, 
			/*      */ &status); // pivot vector
	
	if(status != 0) {
		std::cout << "!!!!!!!!!!!!!!!!!dgetrf returned" << status << std::endl;
	}
    // 2.1
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasLower,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasUnit,
			/* M     */ m,
			/* N     */ k,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ matrix+m*n,
			/* LDB   */ n);
	std::cout << "2.1 ok" << std::endl;
    // 2.2
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasUpper,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasUnit,
			/* M     */ m,
			/* N     */ k,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ matrix+m*n,
			/* LDB   */ n);
	std::cout << "2.2 ok" << std::endl;
    // 3.1
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasLower,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasUnit,
			/* M     */ m,
			/* N     */ 1,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ rhs,
			/* LDB   */ n);
	std::cout << "3.1 ok" << std::endl;
    // 3.2
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasUpper,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasUnit,
			/* M     */ m,
			/* N     */ 1,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ rhs,
			/* LDB   */ n);
	std::cout << "3.2 ok" << std::endl;
    // 4.1 
	cblas_dgemm(CblasColMajor,
			/* TRANSA */ CblasNoTrans,
			/* TRANSB */ CblasNoTrans,
			/* M      */ k,
			/* N      */ k,
			/* K      */ m,
			/* ALPHA  */ -1.0,
			/* A      */ matrix+m,
			/* LDA    */ n,
			/* B      */ matrix+m*n,
			/* LDB    */ n,
			/* BETA   */ 1.0,
			/* C      */ matrix+n*m+m,
			/* LDC    */ n);
	std::cout << "4.1 ok" << std::endl;
	cblas_dgemv(CblasColMajor,
			/* TRANS */ CblasNoTrans,
			/* M     */ k,
			/* N     */ m,
			/* ALPHA */ -1.0,
			/* A     */ matrix+m,
			/* LDA   */ n,
			/* X     */ rhs,
			/* INCX  */ 1,
			/* BETA  */ 1.0,
			/* Y     */ rhs+m,
			/* INCY  */ 1);
	std::cout << "4.2 ok" << std::endl;
	/* TODO: Comment this for production */
	for(int i = 0; i < m; i++) 
		for(int j = 0; j < i; j++) 
			matrix[j*n+i] = 0.0;
	for(int i = m; i < n; i++) 
		for(int j = 0; j < m; j++) 
			matrix[j*n+i] = 0.0;
}

void solve(double * matrix, double * rhs, int n) {
    std::vector<int> ipiv(n);
	int status;
	int nrhs = 1;
	dgesv_(
		/* N     */ &n,
		/* NRHS  */ &nrhs,
		/* A     */ matrix,
		/* LDA   */ &n,
		/* IPIV  */ &*ipiv.begin(),
		/* B     */ rhs,
		/* LDB   */ &n,
		/* INFO  */ &status
	);
	printf("solve status: %d\n", status);
}

/*
			
	A1:                               A2:
	+---+---+       +---+             +---+---+      +---+
	| A | B |}n     | E |}n           | G | H |      | K |
	+---+---+       +---+             +---+---+      +---+
	| C | D |}n     | F |}n           | I | J |      | L |
	+---+---+       +---+             +---+---+      +---+
	  ^   ^
	  n   n

			||||
			\||/
			 \/

	+---+---+---+      +---+           +---+---+---+      +---+
	|D+G| C | H |}n    |F+K|}n         | A'| B'| C'|      | K'|
	+---+---+---+      +---+     ===   +---+---+---+      +---+
	| B | A | 0 |}n    | E |}n   ===   | D'| E'| F'|      | L'|
	+---+---+---+      +---+           +---+---+---+      +---+
	| I | 0 | J |}n    | L |}n         | G'| I'| J'|      | M'|
	+---+---+---+      +---+           +---+---+---+      +---+

*/

void copy2(double * A, double * B, int n, int lda, int ldb) {
	double *ptrA, *ptrB;
	for(int i = 0; i < n; i++) {
		ptrA = A + i * lda;
		ptrB = B + i * ldb;
		memcpy(ptrA, ptrB, n*sizeof(double));
	}
}

void zero2(double * A, int n, int lda) {
	double *ptrA, *ptrB;
	for(int i = 0; i < n; i++) {
		ptrA = A + i * lda;
		memset(ptrA, 0, n*sizeof(double));
	}
}
void add2(double * A, double * B, int n, int lda, int ldb) {
	double *ptrA, *ptrB;
	for(int i = 0; i < n; i++) {
		ptrA = A + i * lda;
		ptrB = B + i * ldb;
		for(int j = 0; j < n; j++) {
			ptrA[j] += ptrB[j];
		}
	}
}

void add(double * A1, double * A2, double * B1, double * B2, int n, double * outA, double * outB) {
	double 
		* A = A1,
		* B = A1 + n,
		* C = A1 + 2*n*n,
		* D = A1 + 2*n*n + n,
		
		* E = B1,
		* F = B1 + n,
		
		* G = A2,
		* H = A2 + n,
		* I = A2 + 2*n*n,
		* J = A2 + 2*n*n + n,
		
		* K = B2,
		* L = B2 + n,
		
		* oA = outA,
		* oB = outA +             n,
		* oC = outA +           2*n,
		* oD = outA +   3*n*n,
		* oE = outA +   3*n*n +   n,
		* oF = outA +   3*n*n + 2*n,
		* oG = outA + 2*3*n*n,
		* oH = outA + 2*3*n*n +   n,
		* oI = outA + 2*3*n*n + 2*n,
		
		* oK = outB,
		* oL = outB + n,
		* oM = outB + 2*n;
		
	/*
		oA = D+G
		oB = C
		oC = H
		oD = B
		oE = A
		oG = I
		oI = J
		
		oK = F + K
		oL = E
		oM = L
	*/
	
	copy2(oA, D, n, 3*n, 2*n);
	 add2(oA, G, n, 3*n, 2*n);

	copy2(oB, C, n, 3*n, 2*n);
	copy2(oC, H, n, 3*n, 2*n);
	copy2(oD, B, n, 3*n, 2*n);
	copy2(oE, A, n, 3*n, 2*n);
	zero2(oF, n, 3*n);
	copy2(oG, I, n, 3*n, 2*n);
	zero2(oH, n, 3*n);
	copy2(oI, J, n, 3*n, 2*n);
	
	
	memcpy(oK, F, n*sizeof(double));
	for(int i = 0; i < n; i++)
		oK[i] += K[i];
	
	memcpy(oL, E, n*sizeof(double));
	memcpy(oM, L, n*sizeof(double));
	
}

void testAdd() {
	
	double A[4][4] = {
			{ 11,  12, 21,  22 },
			{ 13,  14, 23,  24 },
			{ 31,  32, 41,  42 },
			{ 33,  34, 43,  44 }
	};
	
	double B[4][4] = {
		{ 1100,  1200, 2100,  2200 },
		{ 1300,  1400, 2300,  2400 },
		{ 3100,  3200, 4100,  4200 },
		{ 3300,  3400, 4300,  4400 }
	};
	
	double C[6][6];
	
	double Ar[4] = {11,12,21,22};
	double Br[4] = {3100,3200,4100,4200};
	double Cr[6];

	add(A[0],B[0],Ar,Br,2,C[0],Cr);
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++) 
            printf("%4.0f  \t", C[i][j]);
	    printf(" | %f  ", Cr[i]);	
        printf("\n");
    }
	
}
