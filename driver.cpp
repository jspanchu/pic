#include <iostream>
#include <cstdlib>
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include "include/poisson_solver.hpp"

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

    pPlasma->setNodes(10000);
    pPlasma->setGridWidth(pCharges->getL());
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
    pPlasma->calcDensity();

    PoissonSolver *pSystem;
    PoissonSolver system(pPlasma,pCharges);
    pSystem = &system;

    pSystem->setPhi(pPlasma);
    pSystem->setE();
    pSystem->setLocalE(pPlasma,pCharges);
	
    for(int i = 0; i < pPlasma->getNodes(); ++i)
    {
        std::cout << i << "," << pSystem->getPhi(i) << std::endl;
    }

}







