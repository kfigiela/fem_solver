#!/bin/bash
set -x
make

#1
./merge 1 mA1.txt mA.txt test/state1.txt test/out1.txt
#2
./merge 1 mA.txt  mA.txt test/state2.txt test/out2.txt
#3
./merge 1 mA.txt  mA.txt test/state3.txt test/out3.txt
#4
./merge 1 mA.txt mAN.txt test/state4.txt test/out4.txt

#5
./merge 1 test/out1.txt test/out2.txt test/state5.txt test/out5.txt
#6
./merge 1 test/out3.txt test/out4.txt test/state6.txt test/out6.txt

#7 - root
./solve 1 test/out5.txt test/out6.txt test/rhs5.txt test/rhs6.txt

#5
./down 1 test/state5.txt test/rhs5.txt test/rhs1.txt test/rhs2.txt
#6
./down 1 test/state6.txt test/rhs6.txt test/rhs3.txt test/rhs4.txt

#1
./down 1 test/state1.txt test/rhs1.txt test/_rhs1L.txt test/_rhs1R.txt
#2
./down 1 test/state2.txt test/rhs2.txt test/_rhs2L.txt test/_rhs2R.txt
#3
./down 1 test/state3.txt test/rhs3.txt test/_rhs3L.txt test/_rhs3R.txt
#4
./down 1 test/state4.txt test/rhs4.txt test/_rhs4L.txt test/_rhs4R.txt

