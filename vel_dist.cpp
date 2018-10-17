#include "include/vel_dist.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
VelDist::VelDist()
{
	//set seed, beam velocity, number of ions/electrons and thermal spread.
	srand(time(NULL));
	//pointers to f,v,x
	this->pX = new double[n_0];
	this->pF = new double[n_0];
	this->pV = new double[n_0];	
	this->pPositionElec = new double[n_0];
	this->pPositionIon  = new double[n_0];
}
VelDist::~VelDist()
{
	delete this->pX;
	delete this->pV;
	delete this->pF;
	delete this->pPositionElec;
	delete this->pPositionIon;
}

//Getters start.
int VelDist::getN()
{
	return this->n_0;
}
double VelDist::getL()
{
	return this->L;
}
double VelDist::getX(int i)
{
	return *(pX+i);
}
double VelDist::getF(int i)
{
	return *(pF+i);
}
double VelDist::getV(int i)
{
	return *(pV+i);
}
double VelDist::getTolerance()
{
	return this->tolerance;
}
double VelDist::getPositionElec(int i)
{
	return *(pPositionElec+i);
}
double VelDist::getPositionIon(int i)
{
	return *(pPositionIon+i);
}
//Setters start.
void VelDist::setN(int n0)
{
	if (n0 < 0)
	{
		std::cout << "Error! number of electrons can't be negative!";
	}
	else
	{
		this->n_0 = n0;
		//re-allocate for new number of electrons.
		delete this->pX;
		delete this->pF;
		delete this->pV;
		delete this->pPositionElec;
		delete this->pPositionIon;
		this->pX = new double[n_0];
		this->pF = new double[n_0];
		this->pV = new double[n_0];
		this->pPositionElec = new double[n_0];
		this->pPositionIon  = new double[n_0];
	}
}
void VelDist::setL(double L)
{
	this->L = L;
}
void VelDist::setV_th(double Vth)
{
	this->v_th = Vth;
}
void VelDist::setV_b(double Vb)
{
	this->v_b = Vb;
}

void VelDist::setVbounds()
{
	//v_max = allowable velocity of the farther edge of maxwellian with high velocity.
	//v_min = allowable velocity of the lower edge of maxwellian with low velocity.
	// DO NOT SET THE FACTOR = 1. SET IT SOMEWHERE B/W 1.1 AND 10.
	this->v_max = 4*v_b;
	this->v_min = -4*v_b;
}
void VelDist::setTolerance(double toler)
{
	this->tolerance = toler;
}
void VelDist::setX()
{
	//Check and rectify this whole function.
	double elecElecTolerance = 0.;
	double ionIonTolerance = 0.;
	double ionElecTolerance = 0.;

	for (int i = 0; i < getN(); ++i)
	{
		this->generateX(i);
		if(i != 0)
		{
			elecElecTolerance = std::abs(getX(i) - getX(i-1)) /getX(i);
		}
		while (elecElecTolerance < this->getTolerance())
		{
			std::cout << "generating X for electrons..." << std::endl;
			this->generateX(i);
			elecElecTolerance = std::abs(getX(i) - getX(i+1))/getX(i);
		}
		*(pPositionElec+i) = *(pX+i);
	
	}
	std::cout << "Finished " << std::endl;
	for (int i = 0; i < getN(); ++i)
	{
		this->generateX(i);
		if(i != 0)
		{
			ionIonTolerance = std::abs(getX(i) - getX(i-1))/getX(i);
		}
		while (ionIonTolerance < this->getTolerance())
		{
			std::cout << "generating X for ions..." << std::endl;
			this->generateX(i);
			ionIonTolerance = std::abs(getX(i) - getX(i+1))/getX(i);
			ionElecTolerance = std::abs(getX(i) - this->getPositionElec(i))/getX(i);
		}
		*(pPositionIon+i) = *(pX+i);		
	}
	std::cout << "Finished " << std::endl;

}
//General functions start.
void VelDist::show()
{
	std::cout << "v_b = " << this->v_b << std::endl;
	std::cout << "v_max = " << this->v_max << std::endl;
	std::cout << "v_min = " << this->v_min << std::endl;
	std::cout << "v_th = " << this->v_th << std::endl;
	std::cout << "n_0 = " << this->n_0 << std::endl;

}
void VelDist::generateX(int i)
{
	this->pX[i] = 0 + L * (double) rand() / (double) RAND_MAX;
}
void VelDist::sampleV()
{
	std::cout << "Sampling Velocity..." << std::endl;
	for (int i = 0 ; i < this->getN() ; ++i)
	{
		*(pV+i) = generateV(i);
	}
	std::cout << "Finished " << std::endl;
}
double VelDist::generateV(int i)
{
	//generate random velocity b/w v_min and v_max.
	double v = 0.;
	v = v_min + (v_max - v_min)* (double) rand() / (double) RAND_MAX;
	//return v;
	return generateF(v,i);
}
double VelDist::generateF(double v, int i)
{
	double param1 = 1/(2.*pi*v_th*v_th);
	double param2 = -(v-v_b) * (v-v_b)/(v_th * v_th * 2.);
	double param3 = -(v+v_b) * (v+v_b)/(v_th * v_th * 2.);

	//f(v) for i'th electron
	*(pF+i) = (n_0 * 0.5) * sqrt(param1) * (exp(param2) + exp(param3)) ;

	//FMAX at v = +v_b or -v_b.
	double F_MAX = (n_0 * 0.5) * sqrt(param1) * (1 + exp(double((-2. * v_b * v_b)/(v_th*v_th))));

	//generate random w within 0, F_MAX.
	double w =  F_MAX *  (double) rand() / (double) RAND_MAX;
	return acceptV(w,v,*(pF+i),i);
}
double VelDist::acceptV(double w, double v, double f, int i)
{
	//acceptance of v.
	if (w < f)
	{
		return v;
	}
	//Rejection and regenerate v.
	else
	{
		return generateV(i);
	}
}
















