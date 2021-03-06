#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cout
        << "usage: solve n inLeftMatrix inRightMatrix outLeftRhs outRightRhs"
        << std::endl;
    exit(1);
  }

  int n = atoi(argv[1]);
  int N = 3 * n;

  std::pair<double *, double *> Ap = readMatrixRhs(argv[2], 2 * n);
  double *A = Ap.first;
  double *Ar = Ap.second;

  std::pair<double *, double *> Bp = readMatrixRhs(argv[3], 2 * n);
  double *B = Bp.first;
  double *Br = Bp.second;

  double *C = new double[N * N];
  double *Cr = new double[N];

  add(A, B, Ar, Br, n, C, Cr);
  solve(C, Cr, N);

  writeLeftRhs(argv[4], Cr, N, n);
  writeRightRhs(argv[5], Cr, N, n);

  return 0;
}
