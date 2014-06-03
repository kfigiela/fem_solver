CXX_FLAGS=-DYA_BLAS -DYA_LAPACK -DYA_BLASMULT
LIBS= -lblas -llapack -framework vecLib
CXX=clang++

SRCS = main.cpp io.cpp merge.cpp
OBJS = $(SRCS:.c=.o)
MAIN = main

.PHONY: depend clean

all:	$(MAIN)
		@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS) 
		$(CXX) $(CFXXLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
		$(CXX) $(CFXXLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it