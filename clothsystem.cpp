#include "clothsystem.h"
//Spring Damper
#define spring_constant 3000
#define damp_constant 5
//Aerodynamic Force
#define density .01
#define drag_coefficient .01
#define windvelocity Vector3(0,0,05000)

ClothSystem::ClothSystem(int x, int y){
	unsigned int size = pow(x * y, 2);
	float restlength;

	p = new Particle[x * y];
	char *adjacencymatrix = new char[size]();
	gravity.Set(0, -9.8, 0);
	numparticles = x * y;
	numparticlesx = x;
	numparticlesy = y;

	for (int i = 0; i < numparticlesx; i++){
		for (int j = 0; j < numparticlesy; j++){
			p[i * numparticlesy + j].position.Set((float)i / 1, (float)-j / 1, (float)5/ 1);
		}
	}

	for (int i = 0; i < numparticles; i++){
		adjacencymatrix[i * numparticles + i] = 1;
	}

	int i2, j2;
	for (int i = 0; i < numparticlesx; i++){
		for (int j = 0; j < numparticlesy; j++){
			for (int ix = -1; ix < 2; ix++){
				for (int iy = -1; iy < 2; iy++){
					i2 = ix + i;
					j2 = iy + j;
					
					if (i2 >= 0 && i2 < numparticlesx && j2 >= 0 && j2 < numparticlesy && !adjacencymatrix[(i * numparticlesy + j) * numparticles + (i2 * numparticlesy + j2)]) {
						restlength = (p[i2 * numparticlesy + j2].position - p[i * numparticlesy + j].position).Mag();
						springs.push_back(SpringDamper(spring_constant, damp_constant, restlength, p[i * numparticlesy + j], p[i2 * numparticlesy + j2]));
						adjacencymatrix[(i * numparticlesy + j) * numparticles + (i2 * numparticlesy + j2)] = 1;
						adjacencymatrix[(i2 * numparticlesy + j2) * numparticles + (i * numparticlesy + j)] = 1;
					}
				}
			}
		}
	}

	for (int i = 0; i < (numparticlesx - 1); i++){
		for (int j = 0; j < (numparticlesy - 1); j++){
			drag.push_back(AerodynamicForce(p[i * numparticlesy + j], p[(i + 1) * numparticlesy + j], p[i * numparticlesy + (j + 1)], density, drag_coefficient, windvelocity));
			drag.push_back(AerodynamicForce(p[(i + 1) * numparticlesy + j], p[(i + 1) * numparticlesy + (j + 1)], p[i * numparticlesy + (j + 1)], density, drag_coefficient, windvelocity));
		}
	}

	//overarchign cloth structure - grid like structure

	
	restlength = (p[0].position - p[numparticlesy - 1].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[0], p[numparticlesy - 1]));
	
	restlength = (p[numparticlesy - 1].position - p[numparticlesx * numparticlesy - 1].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[numparticlesy - 1], p[numparticlesx * numparticlesy - 1]));

	restlength = (p[numparticlesx  * numparticlesy - 1].position - p[(numparticlesx - 1) * numparticlesy].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[numparticlesx * numparticlesy - 1], p[(numparticlesx - 1) * numparticlesy]));

	restlength = (p[(numparticlesx - 1) * numparticlesy].position - p[0].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[(numparticlesx - 1) * numparticlesy], p[0]));

	restlength = (p[0].position - p[numparticlesx * numparticlesy - 1].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[0], p[numparticlesx * numparticlesy - 1]));

	restlength = (p[numparticlesy - 1].position - p[(numparticlesx - 1) * numparticlesy].position).Mag();
	springs.push_back(SpringDamper(1000, 1, restlength, p[numparticlesy - 1], p[(numparticlesx - 1) * numparticlesy]));

	delete[] adjacencymatrix;
}

void ClothSystem::draw(){
	//int index = 0;
	Vector3 normal;
	int tri_index1, tri_index2, tri_index3, tri_index4;
	int subindex1, subindex2;

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	for (int i = 0; i < (numparticlesx - 1); i++){
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		subindex1 = i * numparticlesy;
		subindex2 = (i + 1) * numparticlesy;
		for (int j = 0; j < (numparticlesy - 1); j++){
			tri_index1 = subindex1 + j;
			tri_index2 = subindex2 + j;
			tri_index3 = subindex1 + (j + 1);
			tri_index4 = subindex2 + (j + 1);

			//glNormal3f(drag[index].normal.x, drag[index].normal.y, drag[index].normal.z);			
			//index++;
			normal = vertex_normal(i, j);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index1].position.x, p[tri_index1].position.y, p[tri_index1].position.z);
			normal = vertex_normal(i + 1, j);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index2].position.x, p[tri_index2].position.y, p[tri_index2].position.z);
			normal = vertex_normal(i, j + 1);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index3].position.x, p[tri_index3].position.y, p[tri_index3].position.z);

			//glNormal3f(drag[index].normal.x, drag[index].normal.y, drag[index].normal.z);
			//index++;
			normal = vertex_normal(i + 1, j);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index2].position.x, p[tri_index2].position.y, p[tri_index2].position.z);
			normal = vertex_normal(i + 1, j + 1);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index4].position.x, p[tri_index4].position.y, p[tri_index4].position.z);
			normal = vertex_normal(i, j + 1);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p[tri_index3].position.x, p[tri_index3].position.y, p[tri_index3].position.z);
		}
		glEnd();
	}
	
	glPopMatrix();
}

Vector3 ClothSystem::vertex_normal(int x, int y){
	int newI, newJ, counter = -2;
	Vector3 normal, temp, vectors[4], particle_pos = p[x * numparticlesy + y].position;

	for (int j = -1; j < 2; j += 2){
		newI = x;
		newJ = y + j;
		if (newI >= 0 && newI < numparticlesy && newJ >= 0 && newJ < numparticlesy){

			counter += 2;
			vectors[counter] = p[newI * numparticlesy + newJ].position - particle_pos;
			

		}
	}

	counter = -1;
	for (int i = -1; i < 2; i += 2){
		newI = x + i;
		newJ = y;
		if (newI >= 0 && newI < numparticlesy && newJ >= 0 && newJ < numparticlesy){

			counter += 2;
			vectors[counter] = p[newI * numparticlesy + newJ].position - particle_pos;
			

		}
	}
	for (int i = 0; i < counter; i++){
		temp.Cross(vectors[i], vectors[(i + 1)% 4]);

		//temp = temp.Normalize();
		normal += temp;

	}

	normal /= counter;
	normal = -normal.Normalize();

	//system("pause");
	return normal;
}

void ClothSystem::update(float delta){
	for (int ii = 0; ii < 10; ii++){
		for (int i = 0; i < numparticles; i++) {
			p[i].applyforce(gravity * p[i].mass);
		}

		for (int i = 0; i < springs.size(); i++){
			springs[i].computeforce();
		}

		for (int i = 0; i < drag.size(); i++){
			drag[i].computeforce();
		}

		for (int i = 0; i < numparticles; i++){
			if (i % numparticlesy)
				p[i].update(delta / 10);
		}
	}
}