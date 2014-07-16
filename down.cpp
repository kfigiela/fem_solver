#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char ** argv)
{
	
	if(argc != 6) {
		std::cout << "usage: " << argv[0] << " N inState inRhs outRhs" << std::endl;
		exit(1);
	}

	int n = atoi(argv[1]);
	int N = 3*n;
	
	double* C = new double[N*N];
	double* Cr = new double[N];

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++)
			C[j*N+i] = 0.0;
		C[i*N+i] = 1.0;
	}
	
	std::fstream state(argv[2], std::ios_base::in);

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < N; j++)
			state >> C[j*N+i];
		state >> Cr[i];
	}

	std::fstream rhs(argv[3], std::ios_base::in);

	for(int i = 0; i < 2*n; i++) {
		rhs >> Cr[n+i];
	}


	// writeMatrixRhs(C, Cr, N);
	// std::cout << std::endl;

	solve(C, Cr, N);

	// writeRhs(Cr, N);
	// std::cout << std::endl;

	writeLeftRhs(argv[4], C, Cr, N, n);
	writeRightRhs(argv[5], C, Cr, N, n);


	return 0;
}

