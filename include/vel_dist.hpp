
//This class generates the velocity distribution of two counter-streaming beams of electrons in a hot unmagnetized
//plasma. It makes use of the acceptance-rejection sampling method.

#ifndef VEL_DIST
#define VEL_DIST

class VelDist
{
private:
	const double pi =  3.1415926;
	double v_th = 1.;
	double L = 1000;
	int n_0 = 200000;
	double v_b = 5.;
	double v_min = 0.;
	double v_max = 0.;
	double tolerance = 1e-10;
	double* pX = nullptr;
	double* pV = nullptr;
	double* pF = nullptr;
	double* pPositionElec = nullptr; //would be of size numElectrons
	double* pPositionIon  = nullptr; //would be of size numIons

public:
	VelDist();
	~VelDist();

	//Getters.
	int getN();
	double getL();
	double getX(int);
	double getF(int);
	double getV(int);
	double getTolerance();
	double getPositionElec(int);
	double getPositionIon(int);

	//Setters.
	void setN (int);
	void setL(double);
	void setV_th (double);
	void setV_b  (double);
	void setVbounds();
	void setTolerance(double);
	void setX();

	//General functions
	void show();
	void generateX(int);
	void sampleV();
	double generateV(int);
	double generateF(double, int);
	double acceptV(double,double,double,int);

};
#endif
