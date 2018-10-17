//This class contains the routines to solve poisson's equation.
#include "num_density.hpp"

#ifndef POISSON_SOLVER_HPP_
#define POISSON_SOLVER_HPP_

class PoissonSolver
{
private:
	double* pPhi;
	double* pE;
	double* pLocalE;
	double* plowerDiag;
	double* pmiddleDiag;
	double* pupperDiag;
	double* pnewCoeffC;
	double* pnewCoeffD;

public:
	PoissonSolver();
	~PoissonSolver();
	double getPhi(int);
	double getE(int);
	double getLocalE(int);
	double setDiags();
	double setNewCoeffs();
	double setPhi();
	double setE();
	double setLocalE();


};

#endif