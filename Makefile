CXX = icpc 
OBJS = vel_dist.o file_io.o num_density.o poisson_solver.o particle_mover.o driver.o 
#CXX = g++ -O 3
CXXFLAGS = -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -ldl

all:	tsi

tsi:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -fr tsi $(OBJS) *.out *.png ./imagesOut/*.png ./output/*.csv
