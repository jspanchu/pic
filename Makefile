DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS =-g3 -DDEBUG
else
    CXXFLAGS=-DNDEBUG
endif

OBJS = vel_dist.o file_io.o num_density.o poisson_solver.o driver.o 
CXX = g++ -O3

all:	tsi

tsi:	$(OBJS)
	$(CXX) -o $@ $^

%.o:	%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr vel_dist $(OBJS) *.out *.png
