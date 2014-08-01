#include "io.h"
#include <boost/format.hpp>

boost::format outFormat("%30.25e ");

void readVector(double *vec, std::istream &f, int n) {
  for (int i = 0; i < n; i++)
    f >> vec[i];
}

void readMatrixRhs(double *m, double *rhs, std::istream &f, int lda, int rows,
                   int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
      f >> m[j*lda+i];
    f >> rhs[i];
  }
}

void writeState(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < N; j++)
      f << outFormat % m[j*N+i];
    f << outFormat % rhs[i] << std::endl;
  }
}
void writeExport(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = n; i < N; i++) {
    for (int j = n; j < N; j++)
      f << outFormat % m[j*N+i];
    f << outFormat % rhs[i] << std::endl;
  }
}

void writeRangeRhs(std::ostream &f, double *rhs, int from, int to) {
  for (int i = from; i < to; i++) {
    f << outFormat % rhs[i] << std::endl;
  }
}
