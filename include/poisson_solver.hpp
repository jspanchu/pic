//This class contains the routines to solve poisson's equation.
#include "num_density.hpp"
#include "vel_dist.hpp"
#include "mkl_lapacke.h"

#ifndef POISSON_SOLVER_HPP_
#define POISSON_SOLVER_HPP_

class PoissonSolver
{
private:
	int nodes = 0;
	int numElec = 0;
	int numIon = 0;
	double gridWidth = 0.;
	double* pPhi = nullptr;
	double* pLocalPhi = nullptr;
	double* pE = nullptr;
	double* pLocalE = nullptr;
	double* pdl = nullptr;
	double* pd = nullptr;
	double* pdu = nullptr;

public:
	PoissonSolver(NumDensity*, VelDist*);
	~PoissonSolver();
	
	//Init data for constructor
	void init(NumDensity*,VelDist*);

	//Getters
	double getPhi(int);
	double getLocalPhi(int);
	double getE(int);
	double getLocalE(int);

	//Setters
	void setPhi(NumDensity*);
	void setLocalPhi(VelDist*);
	void setE();
	void setLocalE(VelDist*);
	void setDiags();
	void setNewCoeffs(NumDensity*);
	void clearMem();
	void solver1D_tridiag(double*, double*);
};

#endif