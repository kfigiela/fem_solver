CXX_FLAGS=-DYA_BLAS -DYA_LAPACK -DYA_BLASMULT
LIBS= -lblas -llapack -framework vecLib
CXX=clang++

SRCS = io.cpp operations.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = add.cpp eliminate.cpp merge.cpp

.PHONY: depend clean

all:	add eliminate merge solve down

add: 	add.cpp $(SRCS)
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o add $(SRCS) add.cpp $(LFLAGS) $(LIBS)

eliminate: 	eliminate.cpp $(SRCS)
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o eliminate $(SRCS) eliminate.cpp $(LFLAGS) $(LIBS)

merge: 	merge.cpp $(SRCS)
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o merge $(SRCS) merge.cpp $(LFLAGS) $(LIBS)

solve: 	solve.cpp $(SRCS)
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o solve $(SRCS) solve.cpp $(LFLAGS) $(LIBS)

down: 	down.cpp $(SRCS)
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o down $(SRCS) down.cpp $(LFLAGS) $(LIBS)

.o:
		$(CXX) $(CFXXLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it