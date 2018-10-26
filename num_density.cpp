#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include <iostream>
#include <cmath>
//Constructor
NumDensity::NumDensity()
{
	this->initDensity();
}
//Destructor
NumDensity::~NumDensity()
{	
	this->destroyDensity();
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
	return *(this->pDensity + i);
}

//Setters
void NumDensity::setNodes(int n)
{
	this->nodes = n;
	//re-alloc memory.
	this->destroyDensity();
	this->initDensity();
}
void NumDensity::setGridWidth(double L)
{
	this->gridWidth = L/double(nodes - 1.);
}
void NumDensity::setDensity(double rho, int i)
{
	*(pDensity+i) = rho;
}

//General functions.
void NumDensity::initDensity()
{
	this->pDensity      = new double[this->nodes];
	this->pDensityElec  = new double[this->nodes];
	this->pDensityIon   = new double[this->nodes];
	//gotta initialize all the nodes with charge density 0.0 as the calcDensity functions work to increment
	//elements of the density array.
	for (int i = 0; i < this->nodes; ++i)
	{
		*(pDensityElec + i) = 0.;
		*(pDensityIon+i) = 0.;
		*(pDensity+i) = 0.;
	}
}
void NumDensity::destroyDensity()
{
	delete pDensityElec;
	delete pDensityIon;
	delete pDensity;
}
void NumDensity::calcElecDensity(VelDist* pCharges)
{
	for (int i = 0; i < this->nodes; ++i)
	{
		*(pDensityElec+i) = 0.;
	}
	//Scan all the electrons' positions and 'weight' their positions to their neighbouring nodes.
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		double weight = 0.;
		int nodeID = floor(pCharges->getPositionElec(i) / this->gridWidth);
		weight = double(nodeID+1) - pCharges->getPositionElec(i) / this->gridWidth;
		*(pDensityElec+nodeID) += weight / this->gridWidth;
		if(nodeID == this->nodes - 1)
		{
			*(pDensityElec + 0) += (1. - weight) / this->gridWidth;
		}
		else
		{
			*(pDensityElec+nodeID+1) += (1. - weight) / this->gridWidth;		
		}
	}
}
void NumDensity::calcIonDensity(VelDist* pCharges)
{	//Scan all the Ions' positions and 'weight' their positions to their neighbouring nodes.
	for (int i = 0; i< this->nodes; ++i)
	{
		*(pDensityIon + i) = double (pCharges->getN()) / pCharges->getL();
 	}
}
void NumDensity::calcDensity(VelDist* pCharges)
{
	for (int i = 0; i < this->nodes; ++i) 
	{
		*(pDensity+i) = *(pDensityElec+i) / *(pDensityIon+i) - 1.;
	}
	//std::cout<<*(pDensity + this->nodes-1) << "," << *(pDensity + 0) << std::endl;
}








