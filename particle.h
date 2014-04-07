#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "vector3.h"

class Particle{
public:
	Particle(float m = 1);
	friend class ClothSystem;
	friend class SpringDamper;
	friend class AerodynamicForce;
	friend class Tester;
	void update(float);
	void applyforce(Vector3);
private:
	const float mass;
	Vector3 acceleration, position, velocity, force;
	Vector3 AB_prev_accel, AB_prev_vel;
};

#endif