#include "io.h"
#include <boost/format.hpp>

boost::format outFormat("%30.25e");

std::pair<double *, double *> readMatrixRhs(std::istream &f, int n) {
  double *m, *rhs;
  m = new double[n * n];
  rhs = new double[n];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      f >> m[j * n + i];
    f >> rhs[i];
  }

  return std::make_pair(m, rhs);
}

void writeRhs(std::ostream &f, double *rhs, int n) {
  for (int i = 0; i < n; i++) {
    f << outFormat % rhs[i] << "\n";
  }
}

void writeState(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < N; j++)
      f << outFormat % m[j * N + i] << "\t";
    f << outFormat % rhs[i] << "\n";
  }
}
void writeExport(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = n; i < N; i++) {
    for (int j = n; j < N; j++)
      f << outFormat % m[j * N + i] << "\t";
    f << outFormat % rhs[i] << "\n";
  }
}

void writeLeftRhs(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = n; i < 2 * n; i++) {
    f << rhs[i] << "\n";
  }
  for (int i = 0; i < n; i++) {
    f << rhs[i] << "\n";
  }
}

void writeRightRhs(std::ostream &f, double *m, double *rhs, int N, int n) {
  for (int i = 0; i < n; i++) {
    f << rhs[i] << "\n";
  }
  for (int i = 2 * n; i < 3 * n; i++) {
    f << rhs[i] << "\n";
  }
}
