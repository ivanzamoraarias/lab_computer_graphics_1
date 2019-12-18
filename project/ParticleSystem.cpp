#include "ParticleSystem.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <labhelper.h>
#include <float.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
using namespace glm;

#define PI 3.14159265359

void ParticleSystem::kill(int id)
{
	//std::iter_swap(id, particles.begin() + particles.size());
	particles[id] = particles[particles.size() - 1];

	particles.pop_back();
}

void ParticleSystem::spawn(Particle particle)
{
	int cref_i = this->max_size==NULL?0:this->max_size;
	int cref_size = particles.size();

	if (cref_size < cref_i) {
		particles.push_back(particle);
	}
}

void ParticleSystem::process_particles(float dt, mat4 fighterModelMatrix) // time between the last frame
{
	Particle p;
	p.pos = vec3(fighterModelMatrix * vec4(15.0f,3.5f,0.0f,1.0f));
	p.lifetime = 0;
	p.life_length = 5;
	p.velocity = glm::vec3(0);
	int rate = labhelper::uniform_randf(30,64);
	rate = 64;
	
	for (int i = 0; i < rate; i++) {
		const float theta = labhelper::uniform_randf(0.f, 2.f * PI);
		const float u = labhelper::uniform_randf(0.95f, 1.f);
		const float radius = 40;
		glm::vec3 velocity =
			radius * glm::vec3(u, sqrt(1.f - u * u) * cosf(theta), sqrt(1.f - u * u) * sinf(theta));

		p.velocity = vec3(fighterModelMatrix*vec4(velocity, 1.0f));
		this->spawn(p);
	}
		

	for (unsigned i = 0; i < particles.size(); i++) {
		if (particles[i].lifetime >= particles[i].life_length) {
			kill(i);
			i = i - 1;
		}
	}
	
	for (int i = 0; i < particles.size(); i++) {


		particles[i].lifetime += dt;

		particles[i].pos += (particles[i].velocity * dt);

		
		
		//particles[i].pos = new_pos;
	}
		
}
