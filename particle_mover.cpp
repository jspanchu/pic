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
	this->setIter();
	//ctor
}
ParticleMover::~ParticleMover()
{
	//dtor
}

//Getters.
double ParticleMover::getTmax()
{
	return this->t_max;
}
double ParticleMover::getDt()
{
	return this->dt;
}
int ParticleMover::getIter()
{
	return this->iter;
}
double ParticleMover::getK()
{
	return this->K;
}
double ParticleMover::getU()
{
	return this->U;
}

//Setters
void ParticleMover::setTmax(double t_max)
{
	this->t_max = t_max;
	this->setIter();
}
void ParticleMover::setDt(double dt)
{
	this->dt = dt;
	this->setIter();
}
void ParticleMover::setIter()
{
	this->iter = this->t_max / this->dt;
}
void ParticleMover::xIncr(NumDensity* pPlasma, VelDist* pCharges)
{
	std::cout << "Pushing all the electrons..." << std::endl;
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
	}
	this->t +=dt;
	
}
void ParticleMover::vIncr(PoissonSolver* pSystem, VelDist* pCharges, int k)
{
	std::cout << "Incrementing velocities of all the electrons..." << std::endl;
	for (int i = 0; i < pCharges->getN(); ++i)
	{
		if(k == 1)
		{
			pCharges->setV(pCharges->getV(i) + (dt/2.) * (pSystem->getLocalE(i)),i);
		}
		else
		{	
			pCharges->setV(pCharges->getV(i) - (dt) * (pSystem->getLocalE(i)), i);
		}
	}

}

