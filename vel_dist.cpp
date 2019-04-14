#include "include/vel_dist.hpp"
VelDist::VelDist()
{
	//set seed, beam velocity, number of ions/electrons and thermal spread.
	srand(time(NULL));
	//Init pointers to f,v,x
	this->init();
}
VelDist::~VelDist()
{
	this->destroy();
}

//Getters start.
double VelDist::getV_th()
{
	return this->v_th;
}
double VelDist::getTolerance()
{
	return this->tolerance;
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
		this->destroy();
		this->init();
	}
}
void VelDist::setL(double l)
{
	this->l = l;
}
void VelDist::setV_th(double Vth)
{
	this->v_th = Vth;
}
void VelDist::setV_b(double Vb)
{
	this->v_b = Vb;
	this->setVbounds();
}
//v_max = allowable velocity of the farther edge of maxwellian with high velocity.
//v_min = allowable velocity of the lower edge of maxwellian with low velocity.
//DO NOT SET THE FACTOR = 1. SET IT SOMEWHERE B/W 1.1 AND 10. 
void VelDist::setVbounds()
{
	this->v_max = 4*this->v_b;
	this->v_min = -4*this->v_b;
}
void VelDist::setTolerance(double toler)
{
	this->tolerance = toler;
}

//General functions start.
void VelDist::init()
{
	this->pX = new double[this->n_0];
	this->pF = new double[this->n_0];
	this->pV = new double[this->n_0];	
	this->pPositionElec = new double[this->n_0];
	this->pPositionIon  = new double[this->n_0];
	this->setVbounds();
}
//Sets position of ions and electrons, makes sure no overlap occurs.
void VelDist::initPositions()
{

	double elecElecTolerance = 0.;
	double ionIonTolerance = 0.;
	double ionElecTolerance = 0.;

	std::cout << "generating X for electrons..." << std::endl;
	for (int i = 0; i < this->n_0; ++i)
	{
		
		this->generateX(i);
		if(i != 0)
		{
			elecElecTolerance = std::abs(*(this->pX + i)- *(this->pX + i - 1)) / *(this->pX + i);
		}
		while (elecElecTolerance < this->getTolerance())
		{
			this->generateX(i);
			elecElecTolerance = std::abs(*(this->pX + i) - *(this->pX + i + 1))/ *(this->pX + i);
		}
		*(this->pPositionElec+i) = *(this->pX+i);
	
	}
	std::cout << "Finished " << std::endl;
	std::cout << "generating X for ions..." << std::endl;
	for (int i = 0; i < this->n_0; ++i)
	{
		this->generateX(i);
		if(i != 0)
		{
			ionIonTolerance = std::abs(*(this->pX + i) - *(this->pX + i - 1))/ *(this->pX + i);
		}
		while (ionIonTolerance < this->getTolerance())
		{
			this->generateX(i);
			ionIonTolerance = std::abs(*(this->pX + i) - *(this->pX + i + 1))/ *(this->pX + i);
			ionElecTolerance = std::abs(*(this->pX + i) - *(this->pPositionIon + i)/ *(this->pX + i));
		}
		*(this->pPositionIon+i) = *(this->pX+i);		
	}
	std::cout << "Finished " << std::endl;

}
void VelDist::destroy()
{
	delete this->pX;
	delete this->pF;
	delete this->pV;
	delete this->pPositionElec;
	delete this->pPositionIon;
}
void VelDist::show()
{
	std::cout << "v_th : " << this->v_th << std::endl;
	std::cout << "v_b : " << this->v_b << std::endl;
	std::cout << "v_max : " << this->v_max << std::endl;
	std::cout << "v_min : " << this->v_min << std::endl;
	std::cout << "n_0 : " << this->n_0 << std::endl;
	std::cout << "L : " << this->l << std::endl;

}
void VelDist::generateX(int i)
{
	*(this->pX + i) = l * (double) rand() / (double) RAND_MAX;
}
void VelDist::sampleV()
{
	std::cout << "Sampling Velocity..." << std::endl;
	for (int i = 0 ; i < this->n_0 ; ++i)
	{
		*(this-> pV + i) = this->generateV(i);
	}
	std::cout << "Finished " << std::endl;
}
double VelDist::generateV(int i)
{
	//generate random velocity b/w v_min and v_max.
	double v = 0.;
	v = v_min + (v_max - v_min)* (double) rand() / (double) RAND_MAX;
	//return v;
	return this->generateF(v,i);
}
double VelDist::generateF(double v, int i)
{
	double param1 = 1/(2.*M_PI*v_th*v_th);
	double param2 = -(v-v_b) * (v-v_b)/(v_th * v_th * 2.);
	double param3 = -(v+v_b) * (v+v_b)/(v_th * v_th * 2.);

	//f(v) for i'th electron
	*(pF+i) = (n_0 * 0.5) * sqrt(param1) * (exp(param2) + exp(param3)) ;

	//FMAX at v = +v_b or -v_b.
	double F_MAX = (n_0 * 0.5) * sqrt(param1) * (1. + exp((-2. * v_b * v_b)/(v_th*v_th)));

	//generate random w within 0, F_MAX.
	double w =  F_MAX *  (double) rand() / (double) RAND_MAX;
	return this->acceptV(w,v,*(pF+i),i);
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
		return this->generateV(i);
	}
}
















