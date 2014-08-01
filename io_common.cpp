#include "io.h"
#include <boost/format.hpp>

std::pair<double *, double *> readMatrixRhs(std::istream &f, int lda, int rows,
                                            int cols) {
  double *m, *rhs;
  m = new double[rows * cols];
  rhs = new double[rows];

  readMatrixRhs(m, rhs, f, lda, rows, cols);

  return std::make_pair(m, rhs);
}

std::pair<double *, double *> readMatrixRhs(std::istream &f, int n) {
  return readMatrixRhs(f, n, n, n);
}

std::pair<double *, double *> readMatrixRhs(std::string filename, int n) {
  std::fstream f(filename, std::ios::in | std::ios::binary);
  return readMatrixRhs(f, n, n, n);
}

std::pair<double *, double *> readMatrixRhs(std::string filename, int lda,
                                            int rows, int cols) {
  std::fstream f(filename, std::ios::in | std::ios::binary);
  return readMatrixRhs(f, lda, rows, cols);
}

void readMatrixRhs(double *lhs, double *rhs, std::string filename, int lda,
                   int rows, int cols) {
  std::fstream f(filename, std::ios::in | std::ios::binary);
  readMatrixRhs(lhs, rhs, f, lda, rows, cols);
}

void readVector(double *vec, std::string filename, int n) {
  std::fstream f(filename, std::ios::in | std::ios::binary);
  readVector(vec, f, n);
}

void writeState(std::string filename, double *m, double *rhs, int N, int n) {
  std::fstream f(filename, std::ios_base::out | std::ios::binary);
  writeState(f, m, rhs, N, n);
}
void writeExport(std::string filename, double *m, double *rhs, int N, int n) {
  std::fstream f(filename, std::ios_base::out | std::ios::binary);
  writeExport(f, m, rhs, N, n);
}

void writeLeftRhs(std::string filename, double *rhs, int N, int n) {
  std::fstream f(filename, std::ios_base::out | std::ios::binary);
  writeLeftRhs(f, rhs, N, n);
}

void writeRightRhs(std::string filename, double *rhs, int N, int n) {
  std::fstream f(filename, std::ios_base::out | std::ios::binary);
  writeRightRhs(f, rhs, N, n);
}

void writeLeftRhs(std::ostream &f, double *rhs, int N, int n) {
  writeRangeRhs(f, rhs, n, 2*n);
  writeRangeRhs(f, rhs, 0, n);
}

void writeRightRhs(std::ostream &f, double *rhs, int N, int n) {
  writeRangeRhs(f, rhs, 0, n);
  writeRangeRhs(f, rhs, 2*n, 3*n);
}

void writeRhs(std::ostream &f, double *rhs, int n) {
  writeRangeRhs(f, rhs, 0, n);
}

void writeRhs(double *rhs, int n) { writeRhs(std::cout, rhs, n); }

void writeDebug(double *m, double *rhs, int N) {
  std::ostream &f = std::cout;
  boost::format outFormat("%+3.1e");
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      f << outFormat % m[j * N + i] << " ";
    f << outFormat % rhs[i] << "\n";
  }
}
