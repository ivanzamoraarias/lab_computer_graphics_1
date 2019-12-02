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
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	
	particle.lifetime = millis;

	particles.push_back(particle);
}

void ParticleSystem::process_particles(float dt)
{
	for (int i = 0; i < particles.size(); i++) {
		//if (particles[i].lifetime >= particles[i].life_length) {
		//	kill(i);
		//}
		//else {

			particles[i].lifetime += dt;

			particles[i].pos += (particles[i].velocity*dt);

			const float theta = labhelper::uniform_randf(0.f, 2.f * PI);
			const float u = labhelper::uniform_randf(-1.f, 1.f);
			glm::vec3 velocity = 
			glm::vec3(sqrt(1.f - u * u) * cosf(theta), u, sqrt(1.f - u * u) * sinf(theta));

			particles[i].velocity = velocity;	
		//}
		

	}

	// const float theta = labhelper::uniform_randf(0.f, 2.f * M_PI);
	// const float u = labhelper::uniform_randf(-1.f, 1.f);
	// glm::vec3 pos = glm::vec3(sqrt(1.f - u * u) * cosf(theta), u, sqrt(1.f - u * u) * sinf(theta));
		
}
