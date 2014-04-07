#ifndef _AERODYNAMICFORCE_H
#define _AERODYNAMICFORCE_H

#include "particle.h"

class AerodynamicForce{
public:
	AerodynamicForce(Particle&, Particle&, Particle&, float, float, Vector3);
	void computeforce();
	void computenormal();
	Vector3 normal;
private:
	float density, drag_coeff;
	Vector3 drag_force, wind_velocity;
	Particle *p1, *p2, *p3;
};

#endif