#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char ** argv)
{
	int n = atoi(argv[1]);
	int N = atoi(argv[2]);

	std::pair<double*,double*> Ap = readMatrixRhs(argv[3], N);
	double* A  = Ap.first;
	double* Ar = Ap.second;

	eliminate(A, Ar, N, n);
  // writeMatrixRhs(A, Ar, N);

	return 0;
}

