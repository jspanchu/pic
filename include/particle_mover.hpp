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
    double t_max = 0.;
    double dt = 0.;
    int iter = 0;
    double K = 0.;
    double U = 0.;

public:

    //Constructor
    ParticleMover();
    //Destructor
    ~ParticleMover();


    //Getters
    int getIter();
    double getK();
    double getU();

    //Setters
    void setTime(double);
    void setTimeStep(double);
    void setK(VelDist*);
    void setU(PoissonSolver*,VelDist*);

    //General functions.
    void xIncr(NumDensity*,VelDist*);
    void vIncr(PoissonSolver*,VelDist*, int);
    

};

#endif