#include <fstream>
#include <iostream>

double * readMatrix(std::string filename, int n);
double * readMatrix(std::istream& f, int n);
std::pair<double*, double*> readMatrixRhs(std::string filename, int n);
std::pair<double*, double*> readMatrixRhs(std::istream& f, int n);
double * readVector(std::string filename, int n);
double * readVector(std::istream& f, int n);
void writeMatrix(double * m, int n);
void writeMatrixRhs(double * m, double * rhs, int n);
void writeState(std::string filename, double * m, double * rhs, int N, int n);
void writeState(std::ostream& f, double * m, double * rhs, int N, int n);
void writeExport(std::string filename, double * m, double * rhs, int N, int n);
void writeExport(std::ostream& f, double * m, double * rhs, int N, int n);
void writeLeftRhs(std::ostream&f, double * m, double * rhs, int N, int n);
void writeRightRhs(std::ostream&f, double * m, double * rhs, int N, int n);
void writeLeftRhs(std::string filename, double * m, double * rhs, int N, int n);
void writeRightRhs(std::string filename, double * m, double * rhs, int N, int n);