#include "include/poisson_solver.hpp"
#include "include/num_density.hpp"

PoissonSolver::PoissonSolver()
{
    pPhi 		= new double[nodes];
	pE	 		= new double[nodes];
	pLocalE     = new double[numElec];
	plowerDiag  = new double[nodes];
	pmiddleDiag = new double[nodes];
	pupperDiag  = new double[nodes];
	pnewCoeffC  = new double[nodes-1];
	pnewCoeffD  = new double[nodes];
}
PoissonSolver::~PoissonSolver()
{
    delete pPhi;
	delete pE;
	delete pLocalE;
	delete plowerDiag;
	delete pmiddleDiag;
	delete pupperDiag;
	delete pnewCoeffC;
	delete pnewCoeffD;
}
double PoissonSolver::getPhi(int i)
{
	return *(pPhi+i);
}
double PoissonSolver::getE(int i)
{
	return *(pE+i);
}
double PoissonSolver::getLocalE(int i)
{
	return *(pLocalE+i);
}
void PoissonSolver::setDiags()
{
	//first element of lowerDiag and last element of upperDiag are zero.
	*(plowerDiag+0) = 0.;
	*(pupperDiag+(nodes)-1) = 0;

	for (int i = 0, j = 1, k = 0; i < (nodes), j < (nodes), k < (nodes)-1; ++i, ++j, ++k)
	{	*(plowerDiag+j)  = 1.;
		*(pmiddleDiag+i) = -2.;
		*(pupperDiag+k)  = 1.;
		*(pPhi+i) = 0.;
	}
}
void PoissonSolver::setNewCoeffs()
{
	*(pnewCoeffC+0) = *(pupperDiag+0) / *(pmiddleDiag+0);
	*(pnewCoeffD+0) = (this->getDensity(0)) / *(pmiddleDiag+0);
	for (int i = 1; i < (nodes-1); ++i)
	{
		*(pnewCoeffC+i) = *(pupperDiag+i) / (*(pmiddleDiag+i) - (*(plowerDiag+i) * (*(pnewCoeffC+i-1))));
		//std::cout << "Ci" << *(pnewCoeffC+i) << std::endl;
	}
	for (int j = 1; j < nodes; ++j)
	{
		*(pnewCoeffD+j) = (this->getDensity(j) - (*(plowerDiag+j) * (*(pnewCoeffD+j-1))))/
						(*(pmiddleDiag+j) - (*(plowerDiag+j) * (*(pnewCoeffC+j-1))));
		//std::cout << "Di" << *(pnewCoeffD+j) << std::endl;
	}
}

void PoissonSolver::setPhi()
{
    //you have to multiply rhs by gridWidth^2.
	this->setDiags();
	this->setNewCoeffs();
	//Below are periodic boundary conditions.
	*(pPhi+nodes-1) = 0;
	*(pPhi+0) = 0;
	//perform back-substitution.
	//std::cout << nodes-1 << ","<< *(pPhi+nodes-1) << std::endl;
	for(int i = nodes-2; i > 0; --i)
	{
		*(pPhi+i) = *(pnewCoeffD+i) - (*(pnewCoeffC+i) * (*(pPhi+i+1)));
		//std::cout << i << ","<< *(pPhi+i) << std::endl;
	}
	//std::cout << 0 << ","<< *(pPhi+0) << std::endl;
}
void PoissonSolver::setE()
{
	for(int i = 0; i < (this->getNodes()); ++i)
	{
		//i = 0, i = nodes-1 are boundaries.
		if(i == 0)
		{
			*(pE+0) = (*(pPhi+nodes-2) - *(pPhi+1)) / (2*gridWidth);
		}
		else if(i == nodes-1)
		{
			*(pE+nodes-1) = (*(pPhi+nodes-2) - *(pPhi+1)) / (2*gridWidth);
		}
		else
		{
			*(pE+i) = (*(pPhi+i-1) - *(pPhi+i+1)) / (2*gridWidth);
		}
		//std::cout << i <<","<< *(pE+i) << std::endl;
	}
}
void PoissonSolver::setLocalE()
{
	for(int i = 0; i < (this->getNumElectrons()); ++i)
	{
		nodeCoord = *(pPositionElec+i);
		nodeID = floor(nodeCoord);
		double weight1 = ((nodeCoord - nodeID)/this->getGridWidth());
		double weight2 = ((nodeID+1 - nodeCoord)/this->getGridWidth());
		*(pLocalE+i) = *(pE+nodeID) * weight1 + *(pE+nodeID+1) * weight2;
		//std::cout << i << "," << *(pLocalE+i) << std::endl;
		//reset local variables to zero
		nodeCoord = 0.;
		nodeID = 0;
		weight1 = 0.;
		weight2 = 0.;
	}
}