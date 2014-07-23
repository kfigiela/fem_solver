#include <cstdio>
#include <boost/format.hpp>
#include "operations.h"
#include "io.h"


int main(int argc, char ** argv)
{
	boost::format outFormat("%30.25e");
	
	if(argc != 7) {
		std::cout << "usage: " << argv[0] << " 1|A|N matrixSize interfaceSize inState inRhs outRhs" << std::endl;
		exit(1);
	}

	size_t matrixSize = atoi(argv[2]);
	size_t interfaceSize = atoi(argv[3]);
	size_t interiorSize = matrixSize - 2*interfaceSize;
	
	double* C = new double[matrixSize*matrixSize];
	double* Cr = new double[matrixSize];

	for(int i = 0; i < matrixSize; i++) {
		for(int j = 0; j < matrixSize; j++)
			C[j*matrixSize+i] = 0.0;
		C[i*matrixSize+i] = 1.0;
	}
	
	std::fstream state(argv[4], std::ios_base::in);
	for(int i = 0; i < interiorSize; i++) {
		for(int j = 0; j < matrixSize; j++)
			state >> C[j*matrixSize+i];
		state >> Cr[i];
	}

	std::fstream rhs(argv[5], std::ios_base::in);
	for(int i = interiorSize; i < matrixSize; i++) {
		rhs >> Cr[i];
	}

	solve(C, Cr, matrixSize);
	
	std::fstream f(argv[6], std::ios_base::out);
	switch(argv[1][0]) {
		case '1':
			for(int i = 0; i < matrixSize-interfaceSize; ++i)
				f << outFormat % Cr[i] << std::endl;
			break;
		case 'A':
			for(int i = interiorSize; i < interiorSize+interfaceSize; ++i)
				f << outFormat % Cr[i] << std::endl;
			for(int i = 0; i < interiorSize; ++i)
				f << outFormat % Cr[i] << std::endl;
			break;
		case 'N':
			for(int i = matrixSize-2*interfaceSize; i < matrixSize; ++i)
				f << outFormat % Cr[i] << std::endl;
			for(int i = 0; i < matrixSize-2*interfaceSize; ++i)
				f << outFormat % Cr[i] << std::endl;
			break;
		default:
			std::cerr << "Invalid argument (expected 1, A or N)" << std::endl;
	}
	return 0;
}

