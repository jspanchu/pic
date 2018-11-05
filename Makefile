CXX = icpc 
OBJS = file_io.o driver.o
#CXX = g++ -O 3
CXXFLAGS = -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -ldl

all:	tsi

tsi:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf tsi $(OBJS) *.out *.o ./output/*.csv
