#include <iostream>
#include "operations.h"

#ifdef __APPLE__
extern "C" {
#include "vecLib/clapack.h"
#include "vecLib/cblas.h"
}
#else
#include <cblas.h>
#include <lapacke.h>
#endif

void solve(double *matrix, double *rhs, int n) {
  int ipiv[n];
  int status;
  int nrhs = 1;

  dgesv_(
      /* N     */ &n,
      /* NRHS  */ &nrhs,
      /* A     */ matrix,
      /* LDA   */ &n,
      /* IPIV  */ ipiv,
      /* B     */ rhs,
      /* LDB   */ &n,
      /* INFO  */ &status);
  if (status != 0) {
    std::cerr << "Solve failed, DGESV returned " << status << std::endl;
    throw lapack_exception("DGESV failed");
  }
}

void copy2(double *A, double *B, int n, int lda, int ldb) {
  double *ptrA, *ptrB;
  for (int i = 0; i < n; i++) {
    ptrA = A + i * lda;
    ptrB = B + i * ldb;
    memcpy(ptrA, ptrB, n * sizeof(double));
  }
}
void copy_rc(double *A, double *B, int rows, int cols, int lda, int ldb) {
  double *ptrA, *ptrB;
  for (int i = 0; i < cols; i++) {
    ptrA = A + i * lda;
    ptrB = B + i * ldb;
    memcpy(ptrA, ptrB, rows * sizeof(double));
  }
}

void zero2(double *A, int n, int lda) {
  double *ptrA, *ptrB;
  for (int i = 0; i < n; i++) {
    ptrA = A + i * lda;
    memset(ptrA, 0, n * sizeof(double));
  }
}
void add2(double *A, double *B, int n, int lda, int ldb) {
  double *ptrA, *ptrB;
  for (int i = 0; i < n; i++) {
    ptrA = A + i * lda;
    ptrB = B + i * ldb;
    for (int j = 0; j < n; j++) {
      ptrA[j] += ptrB[j];
    }
  }
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

void add(double *A1, double *A2, double *B1, double *B2, int n, double *outA,
         double *outB) {
  double *A = A1, *B = A1 + 2 * n * n, *C = A1 + n, *D = A1 + 2 * n * n + n,
         *E = B1, *F = B1 + n, *G = A2, *H = A2 + 2 * n * n, *I_ = A2 + n,
         *J = A2 + 2 * n * n + n, *K = B2, *L = B2 + n, *oA = outA,
         *oB = outA + 3 * n * n, *oC = outA + 2 * 3 * n * n, *oD = outA + n,
         *oE = outA + 3 * n * n + n, *oF = outA + 2 * 3 * n * n + n,
         *oG = outA + 2 * n, *oH = outA + 3 * n * n + 2 * n,
         *oI = outA + 2 * 3 * n * n + 2 * n, *oK = outB, *oL = outB + n,
         *oM = outB + 2 * n;

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

  copy2(oA, D, n, 3 * n, 2 * n);
  add2(oA, G, n, 3 * n, 2 * n);

  copy2(oB, C, n, 3 * n, 2 * n);
  copy2(oC, H, n, 3 * n, 2 * n);
  copy2(oD, B, n, 3 * n, 2 * n);
  copy2(oE, A, n, 3 * n, 2 * n);
  zero2(oF, n, 3 * n);
  copy2(oG, I_, n, 3 * n, 2 * n);
  zero2(oH, n, 3 * n);
  copy2(oI, J, n, 3 * n, 2 * n);

  memcpy(oK, F, n * sizeof(double));
  for (int i = 0; i < n; i++)
    oK[i] += K[i];

  memcpy(oL, E, n * sizeof(double));
  memcpy(oM, L, n * sizeof(double));
}

void production_A(double *outA, double *outB, double *inA, double *inB,
                  size_t interfaceSize, size_t matrixSize) {
  size_t interiorSize = matrixSize - 2 * interfaceSize;

  double *A = inA, *B = inA + matrixSize * interfaceSize,
         *C = inA + matrixSize * (interfaceSize + interiorSize),
         *D = inA + interfaceSize,
         *E = inA + matrixSize * interfaceSize + interfaceSize,
         *F = inA + matrixSize * (interfaceSize + interiorSize) + interfaceSize,
         *G = inA + interfaceSize + interiorSize,
         *H = inA + matrixSize * interfaceSize + interfaceSize + interiorSize,
         *I_ = inA + matrixSize * (interfaceSize + interiorSize) +
               interfaceSize + interiorSize,
         *oA = outA, *oB = outA + matrixSize * interiorSize,
         *oC = outA + matrixSize * (interfaceSize + interiorSize),
         *oD = outA + interiorSize,
         *oE = outA + matrixSize * interiorSize + interiorSize,
         *oF =
             outA + matrixSize * (interfaceSize + interiorSize) + interiorSize,
         *oG = outA + interiorSize + interfaceSize,
         *oH = outA + matrixSize * interiorSize + interiorSize + interfaceSize,
         *oI = outA + matrixSize * (interfaceSize + interiorSize) +
               interiorSize + interfaceSize,
         *K = inB, *L = inB + interfaceSize,
         *M = inB + interfaceSize + interiorSize, *oK = outB,
         *oL = outB + interiorSize, *oM = outB + interfaceSize + interiorSize;

  // A<->E
  copy_rc(oA, E, interiorSize, interiorSize, matrixSize, matrixSize);
  copy_rc(oE, A, interfaceSize, interfaceSize, matrixSize, matrixSize);

  // B<->D
  copy_rc(oB, D, interiorSize, interfaceSize, matrixSize, matrixSize);
  copy_rc(oD, B, interfaceSize, interiorSize, matrixSize, matrixSize);

  // G<->H
  copy_rc(oG, H, interfaceSize, interiorSize, matrixSize, matrixSize);
  copy_rc(oH, G, interfaceSize, interfaceSize, matrixSize, matrixSize);

  // C<->F
  copy_rc(oC, F, interiorSize, interfaceSize, matrixSize, matrixSize);
  copy_rc(oF, C, interfaceSize, interfaceSize, matrixSize, matrixSize);

  // I
  copy_rc(oI, I_, interfaceSize, interfaceSize, matrixSize, matrixSize);

  memcpy(oL, K, interfaceSize * sizeof(double));
  memcpy(oK, L, interiorSize * sizeof(double));
  memcpy(oM, M, interfaceSize * sizeof(double));
}

void production_AN(double *outA, double *outB, double *inA, double *inB,
                   size_t interfaceSize, size_t matrixSize) {
  size_t interiorSize = matrixSize - interfaceSize;
  size_t fakeInterfaceSize = 2 * interfaceSize;
  size_t fakeInteriorSize = matrixSize - 2 * interfaceSize;

  double *A = inA, *B = inA + matrixSize * fakeInterfaceSize,
         *C = inA + fakeInterfaceSize,
         *D = inA + matrixSize * fakeInterfaceSize + fakeInterfaceSize,
         *oA = outA, *oB = outA + matrixSize * fakeInteriorSize,
         *oC = outA + fakeInteriorSize,
         *oD = outA + matrixSize * fakeInteriorSize + fakeInteriorSize,
         *K = inB, *L = inB + fakeInterfaceSize, *oK = outB,
         *oL = outB + fakeInteriorSize;

  // A<->D
  copy_rc(oA, D, fakeInteriorSize, fakeInteriorSize, matrixSize, matrixSize);
  copy_rc(oD, A, fakeInterfaceSize, fakeInterfaceSize, matrixSize, matrixSize);

  // C<->B
  copy_rc(oB, C, fakeInteriorSize, fakeInterfaceSize, matrixSize, matrixSize);
  copy_rc(oC, B, fakeInterfaceSize, fakeInteriorSize, matrixSize, matrixSize);

  memcpy(oL, K, fakeInterfaceSize * sizeof(double));
  memcpy(oK, L, fakeInteriorSize * sizeof(double));
}