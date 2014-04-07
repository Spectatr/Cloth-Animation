#include "springdamper.h"

SpringDamper::SpringDamper(float c, float d, float r, Particle &p01, Particle &p02)
: p1(&p01), p2(&p02), constant(c), damping(d), restlength(r){
}

void SpringDamper::computeforce(){
	Vector3 e = (p2->position - p1->position);
	float l = e.Mag(); 
	Vector3 E = e / l;
	float v1 = E.Dot(p1->velocity);
	float v2 = E.Dot(p2->velocity);
	float forceSD = (-constant * (restlength - l)) + (-damping * (v1 - v2));
	//printf("force %f %f %f\n", forceSD, restlength, l);
	Vector3 newforce = e * forceSD;
	p1->applyforce(newforce);
	p2->applyforce(-newforce);
}