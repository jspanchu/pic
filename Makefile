DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS =-g3 -DDEBUG
else
    CXXFLAGS=-DNDEBUG
endif

OBJS = vel_dist.o file_io.o num_density.o poisson_solver.o particle_mover.o driver.o 
CXX = g++ -O3  -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

all:	tsi

tsi:	$(OBJS)
	$(CXX) -o $@ $^

%.o:	%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr tsi $(OBJS) *.out *.png ./imagesOut/*.png ./output/*.out
