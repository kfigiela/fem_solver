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

	double* C = new double[matrixSize*matrixSize];
	double* Cr = new double[matrixSize];

	production_A(C, Cr, A, Ar, interfaceSize, matrixSize);
	
	eliminate(C, Cr, matrixSize, interiorSize);

	writeState(argv[4], C, Cr, matrixSize, interiorSize);	
	writeExport(argv[5], C, Cr, matrixSize, interiorSize);
	
	return 0;
}

