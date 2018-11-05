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
	std::cout << "Pushing all the electrons...\n";
	for (int i = 0; i < pCharges->n_0; ++i)
	{
		//std::cout << i << std::endl;
		*(pCharges->pPositionElec + i) += dt * *(pCharges->pV + i);
		if(*(pCharges->pPositionElec + i) > pCharges->l)
		{
			*(pCharges->pPositionElec + i) -= pCharges->l;
		}
		else if(*(pCharges->pPositionElec + i) < 0.)
		{
			*(pCharges->pPositionElec + i) += pCharges->l;
		}
	}
	this->t +=dt;
	
}
void ParticleMover::vIncr(PoissonSolver* pFields, VelDist* pCharges, int k)
{
	std::cout << "Incrementing velocities of all the electrons...\n";
	for (int i = 0; i < pCharges->n_0; ++i)
	{
		if(k == 1)
		{
			*(pCharges->pV + i) += (dt/2.) * *(pFields->pLocalE + i);
		}
		else
		{	
			*(pCharges->pV + i) -= (dt) * *(pFields->pLocalE + i);
		}
	}

}

