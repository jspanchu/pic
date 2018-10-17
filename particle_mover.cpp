/*
 * particle_mover.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: jaswant
 */

#include "particle_mover.hpp"
#include <cmath>
#include <iostream>

ParticleMover::ParticleMover()
{
	t = -2.5e-5; // in terms of 1 / omega = sqrt((m*eps) / (500000*e*e))  ~ 2.506590041516934e-05
	dt = -2.5e-8; // in terms of 1 / omega = sqrt((m*eps) / (500000*e*e)) ~ 2.506590041516934e-05
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
void ParticleMover::setTime(VelDist* pChargeDist, double fac)
{
	t = fac * sqrt((pChargeDist->getM() * eps) / (pChargeDist->getN()*e*e));

}
void ParticleMover::setTimeStep(VelDist* pChargeDist, double fac)
{
	dt = fac * sqrt((pChargeDist->getM() * eps) / (pChargeDist->getN()*e*e));
}
void ParticleMover::xIncr(NumDensity* pPlasmaSys, VelDist* pChargeDist)
{
	std::cout << "Pushing all the electrons." << std::endl;
	for (int i = 0; i < pPlasmaSys->getNumElectrons(); ++i)
	{
		//std::cout << i << std::endl;
		pPlasmaSys->setPositionElec(pPlasmaSys->getPositionElec(i) + dt * (pChargeDist->getV(i)), i);
		//std::cout <<pPlasmaSys->getPositionElec(i) << std::endl;
	}
}
void ParticleMover::vIncr(NumDensity* pPlasmaSys, VelDist* pChargeDist)
{
	std::cout << "Incrementing velocities of all the electrons." << std::endl;
	for (int i = 0; i < pPlasmaSys->getNumElectrons(); ++i)
	{
		//std::cout << i << std::endl;
		pChargeDist->setV(pChargeDist->getV(i) + dt * e * (pPlasmaSys->getLocalE(i)) / pChargeDist->getM(), i);
	}

}
