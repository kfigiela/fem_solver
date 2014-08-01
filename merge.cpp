#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cout << "usage: solve inLeftMatrix inRightMatrix outState outMatrix"
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

  eliminate(C, Cr, N, n);

  writeDebug(C, Cr, N);

  writeState(argv[4], C, Cr, N, n);
  writeExport(argv[5], C, Cr, N, n);
  return 0;
}
