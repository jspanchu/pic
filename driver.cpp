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
    char option;
    double v_th = 0.;
    double v_b = 0.;
    int n_0 = 0;
    int nodes = 0;
    double t_max = 0.;
    double dt = 0.;

    VelDist *pCharges;
    VelDist counterStream;
    pCharges = &counterStream;

    NumDensity *pPlasma;
    NumDensity plasma;
    pPlasma = &plasma;
    
    ParticleMover* pIterator;
    ParticleMover iterator;
    pIterator = &iterator;

    std::cout << "\n\n#################################" << std::endl;
    std::cout << "Default parameters are : " << std::endl;
    pCharges->show();
    std::cout << "Nodes : " << pPlasma->getNodes() << std::endl;
    std::cout << "t_max : " << pIterator->getTmax() << std::endl;
    std::cout << "dt : " << pIterator->getDt() << std::endl;
    std::cout << "#################################\n\n";
    std::cout << "Accept defaults ? (y/n)" << std::endl;
    std::cin >> option; std::cout << "\n";
    if (option == 'n' | option == 'N')
    {
        std::cout << "\nEnter thermal velocity in debye lengths : " << std::endl;
        std::cin >> v_th;
        pCharges->setV_th(v_th);
        std::cout << "\nEnter beam velocity in terms of thermal velocity : " << std::endl;
        std::cin >> v_b;
        pCharges->setV_b(v_b);
        std::cout << "\nEnter number of charges in the plasma : " << std::endl;
        std::cin >> n_0;
        pCharges->setN(n_0);
        std::cout << "\nEnter number of nodes on the grid : " << std::endl;
        std::cin >> nodes;
        pPlasma->setNodes(nodes);
        std::cout << "\nEnter maximum time (in inverse plasma frequencies) upto which simulation would evolve : " << std::endl;
        std::cin >> t_max;
        pIterator->setTmax(t_max);
        std::cout << "\nEnter time step (in inverse plasma frequencies) : " << std::endl;
        std::cin >> dt;
        pIterator->setDt(dt);
    } 
    else

	pCharges->setVbounds();
    pCharges->sampleV();
	pCharges->setX();
    pPlasma->setGridWidth(pCharges->getL());

    PoissonSolver *pSystem;
    PoissonSolver system(pPlasma,pCharges);
    pSystem = &system;

    for (int k = 0; k <= pIterator->getIter(); ++k)
    {

        std::cout << "\n# Time step : " << k << std::endl;
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
        std::cout << "Wrote Files" << std::endl;
        
        if(k == pIterator->getIter())
        {
            break;
        }
        //Compute density and fields.
        std::cout << "Computing density and fields..." << std::endl;
        calcElecField(pPlasma, pCharges, pSystem);
        
        //Evolve position and velocity.
        pIterator->xIncr(pPlasma,pCharges);
        pIterator->vIncr(pSystem,pCharges,k);
        
    }
    return 0;
}
void calcElecField(NumDensity* pPlasma, VelDist* pCharges, PoissonSolver* pSystem)
{
    std::clock_t t0 = std::clock();
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
    pPlasma->calcDensity(pCharges);

    pSystem->setPhi(pPlasma);
    pSystem->setE();
    pSystem->setLocalE(pCharges);
    std::clock_t t1 = std::clock();
    std::cout << "Time taken : " << double (t1 - t0) / (double) CLOCKS_PER_SEC << "seconds" << std::endl;
}




