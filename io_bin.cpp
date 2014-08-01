#include "io.h"

std::pair<double *, double *> readMatrixRhs(std::istream &f, int lda, int rows, int cols) {
  double *m, *rhs;
  m = new double[rows * cols];
  rhs = new double[rows];

  readMatrixRhs(m, rhs, f, lda, rows, cols);

  return std::make_pair(m, rhs);
}

void readVector(double * vec, std::istream& f, int n) {
  for (int i = 0; i < n; i++)
    f.read(reinterpret_cast<char *>(&vec[i]), sizeof(double));
}

void readMatrixRhs(double * m, double * rhs, std::istream &f, int lda, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
      f.read(reinterpret_cast<char *>(&m[j * lda + i]), sizeof(double));
    f.read(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }  
}


void writeRhs(std::ostream &f, double *rhs, int n) {
  for (int i = 0; i < n; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}

void writeState(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < N; j++)
      f.write(reinterpret_cast<char *>(&m[j * N + i]), sizeof(double));
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}
void writeExport(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = n; i < N; i++) {
    for (int j = n; j < N; j++)
      f.write(reinterpret_cast<char *>(&m[j * N + i]), sizeof(double));
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}

void writeLeftRhs(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = n; i < 2 * n; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
  for (int i = 0; i < n; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}

void writeRightRhs(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = 0; i < n; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
  for (int i = 2 * n; i < 3 * n; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}

void writeRangeRhs(std::ostream &f, double *rhs, int from, int to) {
  for (int i = from; i < to; i++) {
    f.write(reinterpret_cast<char *>(&rhs[i]), sizeof(double));
  }
}