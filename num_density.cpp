#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include <iostream>
#include <cmath>
//Constructor
NumDensity::NumDensity()
{
	this->pDensity      = new double[nodes];
	this->pDensityElec  = new double[nodes];
	this->pDensityIon   = new double[nodes];
	//gotta initialize all the nodes with charge density 0.0 as the calcDensity functions work to increment
	//elements of the density array.
	for (int i = 0; i < nodes; ++i)
	{
		*(pDensityElec+i) = 0.;
		*(pDensityIon+i) = 0.;
		*(pDensity+i) = 0.;
	}
}

//Destructor
NumDensity::~NumDensity()
{
	delete this->pDensityElec;
	delete this->pDensityIon;
	delete this->pDensity;
}

//Getters
double NumDensity::getGridWidth()
{
	return this->gridWidth;
}
int NumDensity::getNodes()
{
	return this->nodes;
}
double NumDensity::getDensity(int i)
{
	return *(pDensity+i);
}

//Setters
void NumDensity::setNodes(int n)
{
	this->nodes = n;
	//re-alloc memory.
	delete this->pDensityElec;
	delete this->pDensityIon;
	delete this->pDensity;
	this->pDensity      = new double[nodes];
	this->pDensityElec  = new double[nodes];
	this->pDensityIon   = new double[nodes];
	//call poissonSolver to re-init its member arrays with new size -> nodes.
}
void NumDensity::setGridWidth(double L)
{
	this->gridWidth = L/nodes;
}
void NumDensity::setDensity(double rho, int i)
{
	*(pDensity+i) = rho;
}

//General functions.
void NumDensity::calcElecDensity(VelDist* pPlasma)
{
	//perform weighting
}
void NumDensity::calcIonDensity(VelDist* pPlasma)
{
	//perform weighting
}
void NumDensity::calcDensity()
{
	for (int i = 0; i < this->getNodes(); ++i) {
		*(pDensity+i) = (*(pDensityElec+i) + *(pDensityIon+i))/(8.85e-12) ;
	}
}








