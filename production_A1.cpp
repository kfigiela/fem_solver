#include <cstdio>
#include "operations.h"
#include "io.h"

int main(int argc, char ** argv)
{
	
	if(argc != 6) {
		std::cout << "usage: " << argv[0] << " matrixSize interfaceSize inMatrix outState outExport" << std::endl;
		exit(1);
	}

	size_t matrixSize = atoi(argv[1]);
	size_t interfaceSize = atoi(argv[2]);
	size_t interiorSize = matrixSize - 2*interfaceSize;
	
	std::pair<double*,double*> Ap = readMatrixRhs(argv[3], matrixSize);
	double* A  = Ap.first;
	double* Ar = Ap.second;
		
	eliminate(A, Ar, matrixSize, interiorSize);
	
	writeState(argv[4], A, Ar, matrixSize, interiorSize);
	writeExport(argv[5], A, Ar, matrixSize, interiorSize);
	
	return 0;
}

