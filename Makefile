#CXX = icpc 
OBJS = vel_dist.o file_io.o num_density.o poisson_solver.o particle_mover.o driver.o
CXX = g++ -O3 
INCLDIRS = -DMKL_ILP64 -m64 -I${MKLROOT}/include
LIBDIRS = -L${MKLROOT}/lib/intel64
CXXFLAGS =  $(INCLDIRS) $(LIBDIRS) -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl
all:	tsi

tsi:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf tsi $(OBJS) ./output/*.csv
