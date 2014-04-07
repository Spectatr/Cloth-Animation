#ifndef _SPRINGDAMPER_H
#define _SPRINGDAMPER_H

#include "particle.h"

class SpringDamper{
public:
	SpringDamper(float, float, float, Particle&, Particle&);
	void computeforce();
private:
	float constant, damping, restlength;
	Particle *p1, *p2;
};


#endif