#include <cstdio>
#include <boost/format.hpp>
#include "operations.h"
#include "io.h"

int main(int argc, char **argv) {
  boost::format outFormat("%30.25e");

  if (argc != 7) {
    std::cout << "usage: " << argv[0]
              << " 1|A|N matrixSize interfaceSize inState inRhs outRhs"
              << std::endl;
    exit(1);
  }

  size_t matrixSize = atoi(argv[2]);
  size_t interfaceSize = atoi(argv[3]);
  size_t interiorSize = matrixSize - 2 * interfaceSize;

  double *C = new double[matrixSize * matrixSize];
  double *Cr = new double[matrixSize];

  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++)
      C[j * matrixSize + i] = 0.0;
    C[i * matrixSize + i] = 1.0;
  }

  readMatrixRhs(C, Cr, argv[4], matrixSize, interiorSize, matrixSize);

  readVector(&Cr[interiorSize], argv[5], matrixSize - interiorSize);

  solve(C, Cr, matrixSize);

  std::fstream f(argv[6], std::ios_base::out);
  switch (argv[1][0]) {
  case '1':
    writeRangeRhs(f, Cr, 0, matrixSize);
    break;
  case 'A':
    writeRangeRhs(f, Cr, interiorSize, interiorSize + interfaceSize);
    // for(int i = interiorSize; i < interiorSize+interfaceSize; ++i)
    //   f << outFormat % Cr[i] << std::endl;
    writeRangeRhs(f, Cr, 0, interiorSize);
    // for(int i = 0; i < interiorSize; ++i)
    //   f << outFormat % Cr[i] << std::endl;
    writeRangeRhs(f, Cr, interiorSize + interfaceSize, matrixSize);
    // for(int i = interiorSize+interfaceSize; i < matrixSize; ++i)
    //   f << outFormat % Cr[i] << std::endl;
    break;
  case 'N':
    writeRangeRhs(f, Cr, matrixSize - 2 * interfaceSize, matrixSize);
    // for(int i = matrixSize-2*interfaceSize; i < matrixSize; ++i)
    //   f << outFormat % Cr[i] << std::endl;
    writeRangeRhs(f, Cr, 0, matrixSize - 2 * interfaceSize);
    // for(int i = 0; i < matrixSize-2*interfaceSize; ++i)
    //   f << outFormat % Cr[i] << std::endl;
    break;
  default:
    std::cerr << "Invalid argument (expected 1, A or N)" << std::endl;
  }
  f.close();
  return 0;
}
