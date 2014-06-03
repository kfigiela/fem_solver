#include <cstring>
#include <cmath>

void eliminate(double * matrix, double * rhs, int n, int m);
void solve(double * matrix, double * rhs, int n);
void add(double * A1, double * A2, double * B1, double * B2, int n, double * outA, double * outB);
void testAdd();