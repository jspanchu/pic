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
	this->gridWidth = (L+1)/nodes;
}
void NumDensity::setDensity(double rho, int i)
{
	*(pDensity+i) = rho;
}

//General functions.
void NumDensity::calcElecDensity(VelDist* pCharges)
{
	//Scan all the electrons' positions and 'weight' their positions to their neighbouring nodes.
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		//Find after which node (nodeID) the electron is positioned.
		nodeCoord = pCharges->getPositionElec(i) / this->gridWidth;
		nodeID = floor(nodeCoord);

		if(nodeID == this->nodes-2)
		{	//If the electron is before the last node, increment the 0'th node. This will be equated to density
			//at last nodeID after the scan is complete to maintain periodicity in density.
			*(pDensityElec + 0) -= (pCharges->getPositionElec(i) - nodeID*this->gridWidth) / (this->gridWidth * this->gridWidth);
			*(pDensityElec + nodeID) -= (nodeID*this->gridWidth + this->gridWidth - pCharges->getPositionElec(i)) / (this->gridWidth * this->gridWidth);
		}
		else
		{
			*(pDensityElec + nodeID + 1) -= (pCharges->getPositionElec(i) - nodeID*this->gridWidth) / (this->gridWidth * this->gridWidth);
			*(pDensityElec + nodeID) -= (nodeID*this->gridWidth + this->gridWidth - pCharges->getPositionElec(i)) / (this->gridWidth * this->gridWidth);
		}
		nodeCoord = 0.;
		nodeID = 0;
	}
	*(pDensityElec + this->nodes-1) = *(pDensityElec + 0);
	std::cout<<*(pDensityElec + this->nodes-1) << *(pDensityElec + 0) << std::endl;
}
void NumDensity::calcIonDensity(VelDist* pCharges)
{	//Scan all the Ions' positions and 'weight' their positions to their neighbouring nodes.
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		//Find after which node (nodeID) the Ion is positioned.
		nodeCoord = pCharges->getPositionIon(i) / this->gridWidth;
		nodeID = floor(nodeCoord);

		if(nodeID == this->nodes-2)
		{	//If the electron is before the last node, increment the 0'th node. This will be equated to density
			//at last nodeID after the scan is complete to maintain periodicity in density.
			*(pDensityIon + 0) += (pCharges->getPositionIon(i) - nodeID*this->gridWidth) / (this->gridWidth * this->gridWidth);
			*(pDensityIon + nodeID) += (nodeID*this->gridWidth + this->gridWidth - pCharges->getPositionIon(i)) / (this->gridWidth * this->gridWidth);
		}
		else
		{
			*(pDensityIon + nodeID + 1) += (pCharges->getPositionIon(i) - nodeID*this->gridWidth) / (this->gridWidth * this->gridWidth);
			*(pDensityIon + nodeID) += (nodeID*this->gridWidth + this->gridWidth - pCharges->getPositionIon(i)) / (this->gridWidth * this->gridWidth);
		}
		nodeCoord = 0.;
		nodeID = 0;
	}
	*(pDensityIon + this->nodes-1) = *(pDensityIon + 0);
	std::cout<<*(pDensityIon + this->nodes-1) << *(pDensityIon + 0) << std::endl;
}
void NumDensity::calcDensity()
{
	for (int i = 0; i < this->nodes; ++i) {
		*(pDensity+i) = *(pDensityElec+i) + *(pDensityIon+i) - 1;
	}
	std::cout<<*(pDensity + this->nodes-1) << *(pDensity + 0) << std::endl;
}








