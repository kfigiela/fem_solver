#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char **argv) {
  int N = atoi(argv[1]);

  std::pair<double *, double *> Ap = readMatrixRhs(argv[2], N);
  double *A = Ap.first;
  double *Ar = Ap.second;

  solve(A, Ar, N);

  writeRhs(Ar, N);

  return 0;
}
