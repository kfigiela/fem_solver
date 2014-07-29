UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXX_FLAGS=-DYA_BLAS -DYA_LAPACK -DYA_BLASMULT
	LDLIBS= -lblas -llapack -framework vecLib
endif

ifeq ($(UNAME_S),Linux)
	CXXFLAGS=-std=c++0x
	LDLIBS=-llapacke -llapack -lblas 
endif

CXX=c++
CC=cc

SRCS = io.cpp operations.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = add.cpp eliminate.cpp merge.cpp test_solve.cpp

BINS=add eliminate merge solve down production_AN production_A1 production_A tsolve down_A test_solve

.PHONY: depend clean ubuntu

# all:	add.out eliminate.out merge.out solve.out down.out production_A.out production_A1.out production_AN.out tsolve.out down_A.out
all: add eliminate merge solve down production_AN production_A1 production_A tsolve down_A test_solve

add: $(OBJS)
eliminate: $(OBJS)
merge: $(OBJS)
solve: $(OBJS)
down: $(OBJS)
production_A1: $(OBJS)
production_A: $(OBJS)
production_AN: $(OBJS)
tsolve: $(OBJS)
down_A: $(OBJS)
test_solve: $(OBJS)

%: %.o $(OBJS)
	$(CXX) $< $(OBJS) -o $@ $(LFLAGS) $(LIBS)
	ln -s $@ $(@:.out=)

.o:
		$(CXX) $(CFXXLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(BINS)

depend: $(SRCS)
		makedepend $(INCLUDES) $^
		
ubuntu:
	sudo apt-get install liblapack-dev liblapacke-dev clang libboost-dev

# DO NOT DELETE THIS LINE -- make depend needs it