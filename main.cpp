#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char ** argv)
{
	int n = atoi(argv[1]);
	int N = 3*n;

	std::pair<double*,double*> Ap = readMatrixRhs(argv[2], 2*n);
	double* A  = Ap.first;
	double* Ar = Ap.second;

	std::pair<double*,double*> Bp = readMatrixRhs(argv[3], 2*n);
	double* B  = Bp.first;
	double* Br = Bp.second;
	
	double* C = new double[N*N];
	double* Cr = new double[N];

	add(A,B,Ar,Br,n,C,Cr);
	writeMatrixRhs(C, Cr, N);
	std::cout << "elimination" << std::endl;
	eliminate(C, Cr, N, n);
	// eliminate(A, Ar, 6, 2);
	// solve(C, Cr, N);
	// solve(A, Ar, 6);

	// writeMatrixRhs(A, Ar, 6);
	writeMatrixRhs(C, Cr, N);

	return 0;
}

