#include <iostream>
#include <cstdlib>
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"

int main(int argc, char **argv) {
    VelDist *pCharges;
    VelDist counterStream;
    pCharges = &counterStream;

    NumDensity *pPlasma;
    NumDensity plasma;
    pPlasma = &plasma;

	pCharges->setVbounds();
    pCharges->sampleV();
	pCharges->setX();

    pPlasma->setNodes(1000);
    pPlasma->setGridWidth(pCharges->getL());
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
	

}







