#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <ctime>
#include <thread>
#include "omp.h"
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include "include/poisson_solver.hpp"
#include "include/particle_mover.hpp"
#include "include/file_io.hpp"


void calcElecField(NumDensity*, VelDist*, PoissonSolver*);
int main(int argc, char **argv) {
    char option,solve,fileOut;
    double v_th = 0.;
    double v_b = 0.;
    int n_0 = 0;
    double l = 0.;
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

    std::cout << "\n\n#################################\n";
    std::cout << "Default parameters are : \n";
    pCharges->show();
    std::cout << "Nodes : " << pPlasma->getNodes();
    std::cout << "\nt_max : " << pIterator->getTmax();
    std::cout << "\ndt : " << pIterator->getDt();
    std::cout << "\n#################################\n";
    std::cout << "\nAccept defaults ? (y/n)\n";
    std::cin >> option;
   if (option == 'n' | option == 'N')
    {
        do
            {
                std::cout << "\nEnter thermal velocity (in normalized units) : \n";
                std::cin >> v_th;
                pCharges->setV_th(v_th);
                std::cout << "Enter beam velocity in terms of thermal velocity : \n";
                std::cin >> v_b;
                pCharges->setV_b(v_b);
                std::cout << "Enter number of charges in the plasma : \n";
                std::cin >> n_0;
                pCharges->setN(n_0);
                std::cout << "Enter the domain's length (in order of debye length's) : \n";
                std::cin >> l;
                pCharges->setL(l);
                std::cout << "Enter number of nodes on the grid : \n";
                std::cin >> nodes;
                pPlasma->setNodes(nodes);
                std::cout << "Enter maximum time (in inverse plasma frequencies) upto which simulation would evolve : \n";
                std::cin >> t_max;
                pIterator->setTmax(t_max);
                std::cout << "Enter time step (in inverse plasma frequencies) : \n";
                std::cin >> dt;
                pIterator->setDt(dt);
                std::cout << "\n#################################\n";
                std::cout << "Entered values :\n";
                pCharges->show();
                std::cout << "\nNodes : " << pPlasma->getNodes();
                std::cout << "\nT_max : " << pIterator->getTmax();
                std::cout << "\nTime step : " << pIterator->getDt();
                std::cout << "\n#################################\n";
                std::cout << "\nSolve ? (y/n) : "; std::cin >> solve; std::cout << "\n";
            }
        while(solve == 'n');
    }
    else
    {
        std::cout << "\nComputing for default values... \n";
    }
    pCharges->initPositions();
    pCharges->sampleV();
    pPlasma->setGridWidth(pCharges->l);

    PoissonSolver *pFields;
    PoissonSolver fields(pPlasma,pCharges);
    pFields = &fields;
    
    int digits = 0;
    int maxDigits = pIterator->getIter();
    do 
    { 
        maxDigits /= 10;
        digits++; 
    } 
    while (maxDigits != 0);
    std::cout << "\nWrite output to file? (y/n) "; std::cin >> fileOut; std::cout << "\n";
    std::clock_t tsolve0 = std::clock();
    for (int k = 0; k <= pIterator->getIter(); ++k)
    {

        std::cout << "\n# Simulation Time : " << k*pIterator->getDt();
        //Compute density and fields.
        std::cout << "\nComputing density and fields...\n";
        calcElecField(pPlasma, pCharges, pFields);

        if(fileOut == 'y')
        {
            //Write fields and positions at current time.
            std::string(j) = std::to_string(k);

            FileIO *pPhi;
            FileIO phi("phi",j,digits);
            pPhi = &phi;
            pPhi->fileWrite("x","phi");
            for(int i = 0; i < pPlasma->getNodes(); ++i)
            {
                pPhi->fileWrite(double(i)*double(pCharges->l)/double(pPlasma->getNodes()), *(pFields->pPhi + i));
            }

            FileIO *pDist;
            FileIO dist("fvx",j,digits);
            pDist = &dist;
            pDist->fileWrite("x","v","f");
            for(int i = 0; i < pCharges->n_0; ++i)
            {

                pDist->fileWrite(*(pCharges->pPositionElec + i), *(pCharges->pV + i), *(pCharges->pF + i));
            }
            FileIO *pDens;
            FileIO dens("rho",j,digits);
            pDens = &dens;
            pDens->fileWrite("x","rho");
            for(int i = 0; i < pPlasma->getNodes(); ++i)
            {
                pDens->fileWrite(double(i)*double(pCharges->l)/double(pPlasma->getNodes()), *(pPlasma->pDensity + i));
            }
            FileIO *pE;
            FileIO field("E",j,digits);
            pE = &field;
            pE->fileWrite("x","E");
            for(int i = 0; i < pPlasma->getNodes(); ++i)
            {
                pE->fileWrite(double(i)*double(pCharges->l)/double(pPlasma->getNodes()),*(pFields->pE + i));
            }

            std::cout << "Wrote Files\n";
        }   
        if(k == pIterator->getIter())
        {
            break;
        }
        
        //Evolve position and velocity.
        pIterator->xIncr(pPlasma,pCharges);
        pIterator->vIncr(pFields,pCharges,k);
        
    }
    std::clock_t tsolve1 = std::clock();
    std::cout << "Time taken to solve : " << double (tsolve1 - tsolve0) / (double) CLOCKS_PER_SEC << "seconds\n";
    return 0;
}
void calcElecField(NumDensity* pPlasma, VelDist* pCharges, PoissonSolver* pFields)
{
    pPlasma->calcElecDensity(pCharges);
    pPlasma->calcIonDensity(pCharges);
    pPlasma->calcDensity(pCharges);

    pFields->calcPhi(pPlasma);
    pFields->calcE();
    pFields->calcLocalE(pCharges);
}




