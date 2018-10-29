//This class is meant to contain all functions that give number density to the nodes of the cell.
#include "vel_dist.hpp"
#include <thread>
#ifndef NUM_DENSITY_HPP_
#define NUM_DENSITY_HPP_

class NumDensity
{
private:
	double gridWidth = 0.;
	int nodes = 1000;
	double nodeCoord = 0.;
	double* pDensityElec;
	double* pDensityIon;
	double* pDensity; //all densities would be of size nodes


public:

    //Constructor
    NumDensity();
    //Destructor
    ~NumDensity();


    //Getters
    double getGridWidth();
    int getNodes();
    double getDensity(int i);

    //Setters
    void setNodes(int);
    void setGridWidth(double);
    void setDensity(double, int);

    //General functions.
    void initDensity();
    void destroyDensity();
    void calcElecDensity(VelDist*);
    void calcIonDensity(VelDist*);
    void calcDensity(VelDist*);

};

#endif