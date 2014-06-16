#include "operations.h"
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
		std::cerr << "!!!!!!!!!!!!!!!!!dgetrf returned" << status << std::endl;
	}
    // 2.1
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasLower,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasNonUnit,
			/* M     */ m,
			/* N     */ k,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ matrix+m*n,
			/* LDB   */ n);
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
    // 3.1
	cblas_dtrsm(CblasColMajor,
			/* SIDE  */ CblasLeft,
			/* UPLO  */ CblasLower,
			/* TRANS */ CblasNoTrans,
			/* DIAG  */ CblasNonUnit,
			/* M     */ m,
			/* N     */ 1,
			/* ALPHA */ 1.0,
			/* A     */ matrix,
			/* LDA   */ n,
			/* B     */ rhs,
			/* LDB   */ n);
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
	
	// /* TODO: Comment this for production */
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
	| I | 0 | J |}n    | L |}n         | G'| H'| I'|      | M'|
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
		* B = A1 + 2*n*n,
		* C = A1 + n,
		* D = A1 + 2*n*n + n,
		
		* E = B1,
		* F = B1 + n,
		
		* G = A2,
		* H = A2 + 2*n*n,
		* I = A2 + n,
		* J = A2 + 2*n*n + n,
		
		* K = B2,
		* L = B2 + n,
		
		* oA = outA,
		* oB = outA +   3*n*n,
		* oC = outA + 2*3*n*n,
		* oD = outA +             n,
		* oE = outA +   3*n*n +   n,
		* oF = outA + 2*3*n*n +   n,
		* oG = outA +           2*n,
		* oH = outA +   3*n*n + 2*n,
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
		oF = 0
		oG = I
		oH = 0
		oI = I
		
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