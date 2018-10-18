#include <iostream>
#include <cstdlib>
#include <string>
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include "include/poisson_solver.hpp"
#include "include/particle_mover.hpp"
#include "include/file_io.hpp"
#include "fstream"

void calcElecField(NumDensity*, VelDist*, PoissonSolver*);
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

    PoissonSolver *pSystem;
    PoissonSolver system(pPlasma,pCharges);
    pSystem = &system;

    ParticleMover* pIterator;
    ParticleMover iterator;
    pIterator = &iterator;

    calcElecField(pPlasma, pCharges, pSystem);
    
    for (int k = 1; k <= pIterator->getIter(); ++k)
    {
        std::cout << "Time step : " << k << std::endl;
        pIterator->xIncr(pPlasma,pCharges);
        pIterator->vIncr(pSystem,pCharges);
        calcElecField(pPlasma, pCharges, pSystem);
        
        std::string(j) = std::to_string(k);
        j.append(".out");
        FileIO *pPhi;
        FileIO phi("./output/phi",j);
        pPhi = &phi;
        pPhi->fileWrite("x","phi");
        for(int i = 0; i < pPlasma->getNodes(); ++i)
        {
            pPhi->fileWrite(i, pSystem->getPhi(i));
        }

        FileIO *pDist;
        FileIO dist("./output/fvx",j);
        pDist = &dist;
        pDist->fileWrite("x","v","f");
        for(int i = 0; i < pCharges->getN(); ++i)
        {
            pDist->fileWrite(pCharges->getPositionElec(i),pCharges->getV(i),pCharges->getF(i));
        }
        FileIO *pDens;
        FileIO dens("./output/rho",j);
        pDens = &dens;
        pDens->fileWrite("x","rho");
        for(int i = 0; i < pPlasma->getNodes(); ++i)
        {
            pDens->fileWrite(i,pPlasma->getDensity(i));
        }
    }



}
void calcElecField(NumDensity* pPlasma, VelDist* pCharges, PoissonSolver* pSystem)
{
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
    pPlasma->calcDensity();

    pSystem->setPhi(pPlasma);
    pSystem->setE();
    pSystem->setLocalE(pPlasma,pCharges);
}




