//This class contains the routines to solve poisson's equation.
#include "num_density.hpp"
#include "vel_dist.hpp"
#include "mkl_lapacke.h"
#include <iostream>

#ifndef POISSON_SOLVER_HPP_
#define POISSON_SOLVER_HPP_

class PoissonSolver
{
private:
	int nodes = 0;
	int numElec = 0;
	int numIon = 0;
	double gridWidth = 0.;
	double* pdl = nullptr;
	double* pd = nullptr;
	double* pdu = nullptr;

public:

	double* pPhi = nullptr;
	double* pLocalPhi = nullptr;
	double* pE = nullptr;
	double* pLocalE = nullptr;

	PoissonSolver(NumDensity*, VelDist*);
	~PoissonSolver();

	//General functions
	void calcPhi(NumDensity*);
	void solver1D_tridiag(double*, double*);
	void calcE();
	void calcLocalE(VelDist*);
	void calcLocalPhi(VelDist*);
};

#endif