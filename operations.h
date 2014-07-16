#include <cstring>
#include <cmath>

void eliminate(double * matrix, double * rhs, int n, int m);
void solve(double * matrix, double * rhs, int n);
void add(double * A1, double * A2, double * B1, double * B2, int n, double * outA, double * outB);
void testAdd();
void production_A(double * outA, double * outB, double * inA, double * inB, size_t interfaceSize, size_t matrixSize);
void production_AN(double * outA, double * outB, double * inA, double * inB, size_t interfaceSize, size_t matrixSize);