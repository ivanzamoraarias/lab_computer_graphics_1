#include "Engine.h"



void Engine::start()
{
	g_window = labhelper::init_window_SDL("3D Engine Battle Zone");

	

	startTime = std::chrono::system_clock::now();
	std::chrono::duration<float> timeSinceStart = 
		std::chrono::system_clock::now() - startTime;

	deltaTime = timeSinceStart.count() - currentTime;
	currentTime = timeSinceStart.count();

	isUpdating = true;
}

bool Engine::update()
{
	std::chrono::duration<float> timeSinceStart = std::chrono::system_clock::now() - startTime;
	deltaTime = timeSinceStart.count() - currentTime;
	currentTime = timeSinceStart.count();

	for (GameObject* object : gameObjects) {
		if (object != nullptr)
			object->update();
	}

	SDL_GL_SwapWindow(g_window);

	return isUpdating;
}

void Engine::addGameObject(GameObject* o)
{
	this->gameObjects.push_back(o);
}

void Engine::cleareGameObjects()
{
	this->gameObjects.clear();
}

float Engine::getDeltaTime()
{
	return deltaTime;
}

float Engine::getCurrentTime()
{
	return currentTime;
}
