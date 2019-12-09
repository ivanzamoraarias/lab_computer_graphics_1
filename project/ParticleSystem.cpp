#include "ParticleSystem.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <labhelper.h>
#include <float.h>

#define PI 3.14159265359

void ParticleSystem::kill(int id)
{
	particles.erase(particles.begin() - id);
	//particles.pop_back;
}

void ParticleSystem::spawn(Particle particle)
{

	particles.push_back(particle);
}

void ParticleSystem::process_particles(float dt) // time between the last frame
{
	for (int i = 0; i < particles.size(); i++) {


		//particles[i].lifetime += dt;

		particles[i].pos += (particles[i].velocity * dt);

		const float theta = labhelper::uniform_randf(0.f, 2.f * PI);
		const float u = labhelper::uniform_randf(-1.f, 1.f);
		glm::vec3 new_pos =
			glm::vec3(sqrt(1.f - u * u) * cosf(theta)*10, u*10, sqrt(1.f - u * u) * sinf(theta)*10);

		particles[i].pos = new_pos;
	}
		
}
