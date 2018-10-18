//This class is meant to contain all functions that push individual particles and increment their velocities in time.
#include "poisson_solver.hpp"
#include "num_density.hpp"
#include "vel_dist.hpp"

#ifndef PARTICLE_MOVER_HPP_
#define PARTICLE_MOVER_HPP_

class ParticleMover
{
private:
	double t = 0.;
    double dt = 0.;
    int iter = 0;

public:

    //Constructor
    ParticleMover();
    //Destructor
    ~ParticleMover();


    //Getters
    int getIter();

    //Setters
    void setTime(double);
    void setTimeStep(double);

    //General functions.
    void xIncr(NumDensity*,VelDist*);
    void vIncr(PoissonSolver*,VelDist*);

};

#endif