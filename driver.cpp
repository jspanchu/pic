#include <iostream>
#include <cstdlib>
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"

int main(int argc, char **argv) {
    VelDist *pPlasma;
    VelDist counterStream;
    pPlasma = &counterStream;

    NumDensity *pCharges;
    NumDensity charges;
    pCharges = &charges;

	pPlasma->setVbounds();
    pPlasma->sampleV();
	pPlasma->setX();

    pCharges->setNodes(1000);
    pCharges->setGridWidth(pPlasma->getL());
    pCharges->calcElecDensity(pPlasma);
    pCharges->calcIonDensity(pPlasma);
	

}







