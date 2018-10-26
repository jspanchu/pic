#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <ctime>
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include "include/poisson_solver.hpp"
#include "include/particle_mover.hpp"
#include "include/file_io.hpp"

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

    for (int k = 0; k <= pIterator->getIter(); ++k)
    {
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
        
        if(k == pIterator->getIter())
        {
            break;
        }
        //Compute density and fields.
        calcElecField(pPlasma, pCharges, pSystem);
        std::cout << "# Time step : " << k << std::endl;
        
        //Evolve position and velocity.
        pIterator->xIncr(pPlasma,pCharges);
        pIterator->vIncr(pSystem,pCharges,k);
        
    }
    return 0;
}
void calcElecField(NumDensity* pPlasma, VelDist* pCharges, PoissonSolver* pSystem)
{
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
    pPlasma->calcDensity(pCharges);

    std::clock_t t0 = std::clock();
    pSystem->setPhi(pPlasma);
    std::clock_t t1 = std::clock();
    std::cout << "Time taken : " << double (t1 - t0) / (double) CLOCKS_PER_SEC << "seconds" << std::endl;
    pSystem->setE();
    pSystem->setLocalE(pCharges);
}




