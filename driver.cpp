#include <chrono>
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
    std::chrono::time_point<std::chrono::system_clock> start, end;

    VelDist *pCharges;
    VelDist counterStream;
    pCharges = &counterStream; //All references to the charge distribution will be made using pCharges

    NumDensity *pPlasma;
    NumDensity plasma;
    pPlasma = &plasma; //All references to the plasma will be made using pPlasma
    
    ParticleMover* pIterator;
    ParticleMover iterator;
    pIterator = &iterator; //All references to the iterator will be made using pIterator

    FileIO *pPhi;
    FileIO *pDist;
    FileIO *pDens;
    FileIO *pE;
    
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
    
    //Start timer
    start = std::chrono::system_clock::now();

    //Evolve the counter-streaming beam in steps of dt.
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

            FileIO phi("phi",j,digits);
            pPhi = &phi;
            pPhi->fileWrite("x","phi");
            for(int i = 0; i < pPlasma->getNodes(); ++i)
            {
                pPhi->fileWrite(double(i)*double(pCharges->l)/double(pPlasma->getNodes()), *(pFields->pPhi + i));
            }

            FileIO dist("fvx",j,digits);
            pDist = &dist;
            pDist->fileWrite("x","v","f");
            for(int i = 0; i < pCharges->n_0; ++i)
            {

                pDist->fileWrite(*(pCharges->pPositionElec + i), *(pCharges->pV + i), *(pCharges->pF + i));
            }

            FileIO dens("rho",j,digits);
            pDens = &dens;
            pDens->fileWrite("x","rho");
            for(int i = 0; i < pPlasma->getNodes(); ++i)
            {
                pDens->fileWrite(double(i)*double(pCharges->l)/double(pPlasma->getNodes()), *(pPlasma->pDensity + i));
            }

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
    //Stop timer.
    end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time : " << elapsed_seconds.count() << " s\n";
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




