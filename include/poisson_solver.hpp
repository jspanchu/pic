//This class contains the routines to solve poisson's equation.
#include "num_density.hpp"
#include "vel_dist.hpp"
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
	double* pE = nullptr;
	double* pLocalE = nullptr;
	double* plowerDiag = nullptr;
	double* pmiddleDiag = nullptr;
	double* pupperDiag = nullptr;
	double* pnewCoeffC = nullptr;
	double* pnewCoeffD = nullptr;

public:
	PoissonSolver(NumDensity*, VelDist*);
	~PoissonSolver();
	
	//Init data for constructor
	void init(NumDensity*,VelDist*);

	//Getters
	double getPhi(int);
	double getE(int);
	double getLocalE(int);

	//Setters
	void setPhi(NumDensity*);
	void setE();
	void setLocalE(NumDensity*, VelDist*);
	void setDiags();
	void setNewCoeffs(NumDensity*);
};

#endif