/*
 * particle_mover.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: jaswant
 */

#include "include/particle_mover.hpp"
#include "include/poisson_solver.hpp"
#include "include/num_density.hpp"
#include "include/vel_dist.hpp"
#include <cmath>
#include <iostream>

ParticleMover::ParticleMover()
{
	this->t = 100.; // in terms of 1 / omega = sqrt((m*eps) / (500000*e*e))  ~ 2.506590041516934e-05
	this->dt = 0.1; // in terms of 1 / omega = sqrt((m*eps) / (500000*e*e)) ~ 2.506590041516934e-05
	this->iter = t / dt;
	//ctor
}
ParticleMover::~ParticleMover()
{
	//dtor
}

int ParticleMover::getIter()
{
	return iter;
}
void ParticleMover::setTime(double t)
{
	this->t = t;

}
void ParticleMover::setTimeStep(double dt)
{
	this->dt = dt;
}
void ParticleMover::xIncr(NumDensity* pPlasma, VelDist* pCharges)
{
	std::cout << "Pushing all the electrons." << std::endl;
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		//std::cout << i << std::endl;
		pCharges->setPositionElec(pCharges->getPositionElec(i) + dt * (pCharges->getV(i)), i);
		if(pCharges->getPositionElec(i) > pCharges->getL())
		{
			pCharges->setPositionElec(pCharges->getPositionElec(i) - pCharges->getL(), i);
		}
		else if(pCharges->getPositionElec(i) < 0.)
		{
			pCharges->setPositionElec(pCharges->getPositionElec(i) + pCharges->getL(), i);
		}
		if (i > pCharges->getN())
		{
			std::cout << "HALT HALT !!" << std::endl;
		}
		//std::cout << i << "..." << pCharges->getPositionElec(i) << std::endl;
	}
	
}
void ParticleMover::vIncr(PoissonSolver* pSystem, VelDist* pCharges)
{
	std::cout << "Incrementing velocities of all the electrons." << std::endl;
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		//std::cout << i << std::endl;
		pCharges->setV(pCharges->getV(i) + dt * (pSystem->getLocalE(i)), i);
	}

}
