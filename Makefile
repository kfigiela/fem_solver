CXX_FLAGS=-DYA_BLAS -DYA_LAPACK -DYA_BLASMULT
LDLIBS= -lblas -llapack -framework vecLib
CXX=clang++


SRCS = io.cpp operations.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = add.cpp eliminate.cpp merge.cpp test_solve.cpp

.PHONY: depend clean

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
		$(RM) *.o *~

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it