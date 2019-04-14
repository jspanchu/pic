#include "include/poisson_solver.hpp"

PoissonSolver::PoissonSolver(NumDensity* pPlasma, VelDist* pCharges)
{
	this->gridWidth = pPlasma->getGridWidth();
	this->nodes = pPlasma->getNodes();
	this->numElec = pCharges->n_0;
    pPhi 		= new double[this->nodes];
	pLocalPhi   = new double[this->numElec];
	pE	 		= new double[this->nodes];
	pLocalE     = new double[this->numElec];
	pdl	= new double[this->nodes-3];
	pd	= new double[this->nodes-2];
	pdu	= new double[this->nodes-3];

	for(int i = 0; i < this->nodes; ++i)
	{
		*(pPhi + i) = 0.;
		*(pLocalPhi + i) = 0.;
		*(pE + i) = 0.;
		*(pLocalE + i) = 0.;
	}
}
PoissonSolver::~PoissonSolver()
{
    delete pPhi;
	delete pLocalPhi;
	delete pE;
	delete pLocalE;
	delete pdl;
	delete pd;
	delete pdu;
}
//General functions
void PoissonSolver::calcPhi(NumDensity* pPlasma)
{
	double *pRhs = nullptr;
	pRhs = new double [this->nodes-2];

	//Neumann boundary conditions.
	*(pPhi + 0) = 0.;
	*(pPhi + this->nodes-1) = 0.;
	
	//Init middle diag and rhs.
	for(int i = 0; i < this->nodes-2; ++i)
	{
		*(pd + i) = -2.;
		*(pRhs + i) = *(pPlasma->pDensity + i + 1);
	}
	//Init lowerdiag and upperdiag.
	for(int i = 0; i < this->nodes-3; ++i)
	{
		*(pdl + i) = 1.;
		*(pdu + i) = 1.;
	}
	solver1D_tridiag(pPhi,pRhs);
	for(int i = 0; i < this->nodes-2; ++i)
	{
		*(pPhi+i+1) = *(pRhs+i) * this->gridWidth * this->gridWidth;
	}
	delete pRhs;
}
void PoissonSolver::solver1D_tridiag(double* pLhs, double* pRhs)
{
    MKL_INT n = this->nodes-2, nrhs = 1, ldb = 1, info;
	info = LAPACKE_dgtsv(LAPACK_ROW_MAJOR, n, nrhs, pdl, pd,
	 	pdu, pRhs, ldb);

	if( info > 0 )
	{
		std::cout << "The diagonal element of the triangular factor of A,\n";
		std::cout << "U(%i,%i) is zero, so that A is singular;\n" << info << info;
		std::cout << "the solution could not be computed.\n";
		exit( 1 );
	}
}
void PoissonSolver::calcE()
{
	for(int i = 0; i < (this->nodes); ++i)
	{
		//i = 0, i = nodes-1 are boundaries.
		if(i == 0)
		{
			*(pE+0) = (*(pPhi+this->nodes-1) - *(pPhi+1)) / 2. /this->gridWidth;
		}
		else if(i == this->nodes-1)
		{
			*(pE+this->nodes-1) = (*(pPhi+this->nodes-2) - *(pPhi+0)) / 2. /this->gridWidth;
		}
		else
		{
			*(pE+i) = (*(pPhi+i-1) - *(pPhi+i+1)) / 2. /this->gridWidth;
		}
	}
}
void PoissonSolver::calcLocalE(VelDist* pCharges)
{
	for(int i = 0; i < this->numElec; ++i)
	{
		double weight = 0.;
		int nodeID = floor(*(pCharges->pPositionElec + i) / this->gridWidth);
		weight = double(nodeID+1) - *(pCharges->pPositionElec + i) / this->gridWidth;
		if(nodeID == this->nodes-1)
		{
			*(pLocalE + 0) = *(pE+nodeID) * weight + *(pE+nodeID+0) * (1.-weight);  
		}
		else
		{
			*(pLocalE + i) = *(pE+nodeID) * weight + *(pE+nodeID+1) * (1.-weight);			
		}
	}
}
void PoissonSolver::calcLocalPhi(VelDist* pCharges)
{
	for(int i = 0; i < this->numElec; ++i)
	{
		double weight = 0.;
		int nodeID = floor(*(pCharges->pPositionElec + i) / this->gridWidth);
		weight = double(nodeID+1) - *(pCharges->pPositionElec + i) / this->gridWidth;
		if(nodeID == this->nodes-1)
		{
			*(pLocalPhi + i) = *(pPhi+nodeID) * weight + *(pPhi+nodeID+0) * (1.-weight);  
		}
		else
		{
			*(pLocalPhi + i) = *(pPhi+nodeID) * weight + *(pPhi+nodeID+1) * (1.-weight);			
		}
	}

}