#include <fstream>
#include <iostream>
#include <stdexcept>

class io_error: public std::runtime_error
{
    public:
        io_error(std::string const& msg):
            std::runtime_error(msg)
        {}
};

double * readMatrix(std::string filename, int n);
double * readMatrix(std::istream& f, int n);
void readVector(double * vec, std::string filename, int n);
void readVector(double * vec, std::istream& f, int n);
std::pair<double*, double*> readMatrixRhs(std::string filename, int n);
std::pair<double*, double*> readMatrixRhs(std::istream& f, int n);
std::pair<double*, double*> readMatrixRhs(std::string filename, int lda, int rows, int cols);
std::pair<double*, double*> readMatrixRhs(std::istream& f, int lda, int rows, int cols);
void readMatrixRhs(double * m, double * rhs, std::istream &f, int lda, int rows, int cols);
void readMatrixRhs(double * lhs, double * rhs, std::string filename, int lda, int rows, int cols);
void writeMatrixRhs(double * m, double * rhs, int n);
void writeRhs(double * rhs, int n);
void writeRhs(std::ostream& f, double * rhs, int n);
void writeState(std::string filename, double * m, double * rhs, int N, int n);
void writeState(std::ostream& f, double * m, double * rhs, int N, int n);
void writeExport(std::string filename, double * m, double * rhs, int N, int n);
void writeExport(std::ostream& f, double * m, double * rhs, int N, int n);
void writeLeftRhs(std::ostream&f, double * rhs, int N, int n);
void writeRightRhs(std::ostream&f, double * rhs, int N, int n);
void writeLeftRhs(std::string filename, double * rhs, int N, int n);
void writeRightRhs(std::string filename, double * rhs, int N, int n);
void writeRangeRhs(std::ostream &f, double *rhs, int from, int to);


void writeDebug(double * m, double * rhs, int N);