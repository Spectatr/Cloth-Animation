#include "aerodynamicforce.h"

AerodynamicForce::AerodynamicForce(Particle &a, Particle &b, Particle &c, float den, float coeff,Vector3 wind)
: p1(&a), p2(&b), p3(&c), density(den), drag_coeff(coeff),wind_velocity(wind){
}

void AerodynamicForce::computeforce(){
	Vector3 t1, t2, t3;
	Vector3 v_surface, v, van;
	v_surface = (p1->velocity + p2->velocity + p3->velocity) / 3;
	v = v_surface - wind_velocity;
	t1 = p2->position - p1->position;
	t2 = p3->position - p1->position;
	t3.Cross(t1, t2);
	normal = t3.Normalize();
	van = ((v.Mag() * (v*t3)) / (2 * t3.Mag())) * t3;
	drag_force = -.5 * density * van * drag_coeff;
	p1->applyforce(drag_force/3);
	p2->applyforce(drag_force/3);
	p3->applyforce(drag_force/3);
}

void AerodynamicForce::computenormal(){
	

}