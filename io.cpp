#include "io.h"

double * readMatrix(std::string filename, int n) {
	std::fstream f(filename, std::ios_base::in);
	return readMatrix(f, n);
}

double * readMatrix(std::istream& f, int n) {
	double* m;
	m = new double[n*n];
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			f >> m[j*n+i];
	}
	
	return m;
}

std::pair<double*, double*> readMatrixRhs(std::string filename, int n) {
	std::fstream f(filename, std::ios_base::in);
	return readMatrixRhs(f, n);
}

std::pair<double*, double*> readMatrixRhs(std::istream& f, int n) {
	double* m, *rhs;
	m = new double[n*n];
	rhs = new double[n];
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			f >> m[j*n+i];
		f >> rhs[i];
	}
	
	return std::make_pair(m,rhs);
}

double * readVector(std::string filename, int n) {
	std::fstream f(filename, std::ios_base::in);
	return readVector(f, n);
}

double * readVector(std::istream& f, int n) {
	double* ret;
	ret = new double[n];
	
	for(int i = 0; i < n; i++)
		f >> ret[i];
	
	return ret;
}

void writeMatrix(double * m, int n) {
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) 
            printf("%f  \t", m[j*n+i]);
        printf("\n");
    }
}

void writeMatrixRhs(double * m, double * rhs, int n) {
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) 
            printf("%4.0f  \t", m[j*n+i]);
        printf(" | %8.0f\n", rhs[i]);
    }
}