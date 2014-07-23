#include <boost/test/minimal.hpp>
#include <boost/format.hpp>
#include <iostream>
#include "operations.h"
#include "io.h"

void test_solve() {
	std::cout << "Testing solve..." << std::endl;
	
	double A[] = {
		1, 1, 0, 1, 0,
		1, 0, 1, 1, 1,
		1, 1, 1, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 1, 0
	};
	
	double B[] = {
		11111, 10100, 1100, 11001, 1110 
	};
	
	solve(A, B, 5);
	
	BOOST_CHECK(fabs(B[0] - 10000) < 1e-6);
	BOOST_CHECK(fabs(B[1] -  1000) < 1e-6);
	BOOST_CHECK(fabs(B[2] -   100) < 1e-6);
	BOOST_CHECK(fabs(B[3] -    10) < 1e-6);
	BOOST_CHECK(fabs(B[4] -     1) < 1e-6);
}

void test_eliminate() {
	std::cout << "Testing eliminate..." << std::endl;
	double A[] = {
		1, 1, 0, 1, 0,
		1, 0, 1, 1, 1,
		1, 1, 1, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 1, 0
	};
	
	double B[] = {
		11111, 10100, 1100, 11001, 1110
	};
	
	eliminate(A, B, 5, 2);
	solve(A, B, 5);

	BOOST_CHECK(fabs(B[0] - 10000) < 1e-6);
	BOOST_CHECK(fabs(B[1] - 1000) < 1e-6);
	BOOST_CHECK(fabs(B[2] -  100) < 1e-6);
	BOOST_CHECK(fabs(B[3] -   10) < 1e-6);
	BOOST_CHECK(fabs(B[4] -    1) < 1e-6);
}

void test_add() {
	std::cout << "Testing add..." << std::endl;
	double A[] = {
		11,  13,  31,  33,
		12,  14,  32,  34,
		21,  23,  41,  43,
		22,  24,  42,  44
	};
	double Ar[] = {1, 2, 3, 4};
	
	double B[] = {
		1100,  1300,  3100,  3300,
		1200,  1400,  3200,  3400,
		2100,  2300,  4100,  4300,
		2200,  2400,  4200,  4400
	};
	double Br[] = {10, 20, 30, 40};
		
	double C[6*6];
	double Cr[6];
	add(A, B, Ar, Br, 2, C, Cr);
		
	double E[] = {
		1141, 1343, 21, 23, 3100, 3300,
		1242, 1444, 22, 24, 3200, 3400,
		  31,   33, 11, 13,    0,    0 ,
		  32,   34, 12, 14,    0,    0,
		2100, 2300,  0,  0, 4100, 4300,
		2200, 2400,  0,  0, 4200, 4400,
	};
	double Er[] = {13, 24, 1, 2, 30, 40};

	for(int i = 0; i < 6*6; ++i)
		if(fabs(C[i] - E[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d LHS does not match") % i).str().c_str());
	for(int i = 0; i < 6; ++i)
		if(fabs(Cr[i] - Er[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d RHS does not match") % i).str().c_str());
}


void test_productionAN() {
	std::cout << "Testing production A1..." << std::endl;

	double A[] = {
		 0,	 1,	 2,	 3,	 4,	 5,	 6,	 7,	 8,	 9,
		10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
		20,	21,	22,	23,	24,	25,	26,	27,	28,	29,
		30,	31,	32,	33,	34,	35,	36,	37,	38,	39,
		40,	41,	42,	43,	44,	45,	46,	47,	48,	49,
		50,	51,	52,	53,	54,	55,	56,	57,	58,	59,
		60,	61,	62,	63,	64,	65,	66,	67,	68,	69,
		70,	71,	72,	73,	74,	75,	76,	77,	78,	79,
		80,	81,	82,	83,	84,	85,	86,	87,	88,	89,
		90,	91,	92,	93,	94,	95,	96,	97,	98,	99
	};
	double Ar[] = {0,1,2,3,4,5,6,7,8,9};
	
	double B[100] = {0};
	double Br[10] = {0};
	
	
	double E[] = {
		88,	89,	80,	81,	82,	83,	84,	85,	86,	87,
		98,	99,	90,	91,	92,	93,	94,	95,	96,	97,
		 8,	 9,	 0,	 1,	 2,	 3,	 4,	 5,	 6,	 7,
		18,	19,	10,	11,	12,	13,	14,	15,	16,	17,
		28,	29,	20,	21,	22,	23,	24,	25,	26,	27,
		38,	39,	30,	31,	32,	33,	34,	35,	36,	37,
		48,	49,	40,	41,	42,	43,	44,	45,	46,	47,
		58,	59,	50,	51,	52,	53,	54,	55,	56,	57,
		68,	69,	60,	61,	62,	63,	64,	65,	66,	67,
		78,	79,	70,	71,	72,	73,	74,	75,	76,	77
	};
	double Er[] = {8,9,0,1,2,3,4,5,6,7};
	production_AN(B, Br, A, Ar, 4, 10);
	

	for(int i = 0; i < 100; ++i)
		if(fabs(B[i] - E[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d LHS does not match") % i).str().c_str());
	for(int i = 0; i < 10; ++i)
		if(fabs(Br[i] - Er[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d RHS does not match") % i).str().c_str());
}

void test_productionA() {
	std::cout << "Testing production A..." << std::endl;

	double A[] = {
		 0,	 1,	 2,	 3,	 4,	 5,	 6,	 7,	 8,	 9,
		10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
		20,	21,	22,	23,	24,	25,	26,	27,	28,	29,
		30,	31,	32,	33,	34,	35,	36,	37,	38,	39,
		40,	41,	42,	43,	44,	45,	46,	47,	48,	49,
		50,	51,	52,	53,	54,	55,	56,	57,	58,	59,
		60,	61,	62,	63,	64,	65,	66,	67,	68,	69,
		70,	71,	72,	73,	74,	75,	76,	77,	78,	79,
		80,	81,	82,	83,	84,	85,	86,	87,	88,	89,
		90,	91,	92,	93,	94,	95,	96,	97,	98,	99
	};
	double Ar[] = {0,1,2,3,4,5,6,7,8,9};
	
	double B[100] = {0};
	double Br[10] = {0};
	
	
	double E[] = {
		44,	45,	40,	41,	42,	43,	46,	47,	48,	49,
		54,	55,	50,	51,	52,	53,	56,	57,	58,	59,
		 4,	 5,	 0,	 1,	 2,	 3,	 6,	 7,	 8,	 9,
		14,	15,	10,	11,	12,	13,	16,	17,	18,	19,
		24,	25,	20,	21,	22,	23,	26,	27,	28,	29,
		34,	35,	30,	31,	32,	33,	36,	37,	38,	39,
		64,	65,	60,	61,	62,	63,	66,	67,	68,	69,
		74,	75,	70,	71,	72,	73,	76,	77,	78,	79,
		84,	85,	80,	81,	82,	83,	86,	87,	88,	89,
		94,	95,	90,	91,	92,	93,	96,	97,	98,	99
	};
	double Er[] = {4,5,0,1,2,3,6,7,8,9};
	production_A(B, Br, A, Ar, 4, 10);
	

	for(int i = 0; i < 100; ++i)
		if(fabs(B[i] - E[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d LHS does not match") % i).str().c_str());
	for(int i = 0; i < 10; ++i)
		if(fabs(Br[i] - Er[i]) > 1e-3) 
			BOOST_FAIL((boost::format("Item #%d RHS does not match") % i).str().c_str());
}


int test_main( int, char *[] ) {
	test_solve();
	test_eliminate();
	test_add();
	test_productionA();
	test_productionAN();

	
	return 0;
}

