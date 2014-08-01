#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cout << "usage: " << argv[0] << " N inState inRhs outRhs" << std::endl;
    exit(1);
  }

  int n = atoi(argv[1]);
  int N = 3 * n;

  double *C = new double[N * N];
  double *Cr = new double[N];

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      C[j * N + i] = 0.0;
    C[i * N + i] = 1.0;
  }

  readMatrixRhs(C, Cr, argv[2], N, n, N);

  readVector(&Cr[n], argv[3], 2 * n);

  // writeMatrixRhs(C, Cr, N);
  // std::cout << std::endl;
  solve(C, Cr, N);

  // writeRhs(Cr, N);
  // std::cout << std::endl;

  writeLeftRhs(argv[4], Cr, N, n);
  writeRightRhs(argv[5], Cr, N, n);

  return 0;
}
