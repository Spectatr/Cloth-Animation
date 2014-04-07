#ifndef _CLOTHSYSTEM_H
#define _CLOTHSYSTEM_H

#include "springdamper.h"
#include "aerodynamicforce.h"
#include <vector>

class ClothSystem{
public:
	ClothSystem(int, int);
	void update(float);
	void draw();
	Vector3 vertex_normal(int x, int y);
	Particle *p;
	float angle = 0;
private:
	int numparticles, numparticlesx, numparticlesy;
	Vector3 gravity;
	std::vector<SpringDamper> springs;
	std::vector<AerodynamicForce> drag;
};

#endif