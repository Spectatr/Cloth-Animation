#include "particle.h"

//Plane
#define y_cord -25
#define elasticity 0
#define friction 0

Particle::Particle(float m)
: mass(m){
	force.Zero();
	
}

void Particle::applyforce(Vector3 f){
	force += f;
}

void Particle::update(float delta){	
	Vector3 acceleration, euler_vel, euler_pos;
	Vector3 AB2_vel, AB2_pos;

	//Two-Step Adam-Bashford integration
	acceleration = force / mass;
	
	euler_vel = velocity + acceleration * delta;
	AB2_vel = euler_vel + delta * (3 / 2 * acceleration - 1 / 2 * AB_prev_accel);
	velocity = AB2_vel;
	
	euler_pos = position + velocity * delta;
	AB2_pos = euler_pos + delta * (3 / 2 * velocity - 1 / 2 * AB_prev_vel);
	position = AB2_pos;

	AB_prev_accel = acceleration;
	AB_prev_vel = velocity;

	if (position.y < y_cord){
		position.y = y_cord + y_cord - position.y;
		velocity.y = -elasticity * velocity.y;
		velocity.x = (1 - friction) * velocity.x;
		velocity.z = (1 - friction) * velocity.z;
	}

	force.Zero();
}