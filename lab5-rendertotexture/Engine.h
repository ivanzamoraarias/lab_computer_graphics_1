#pragma once

#include <GL/glew.h>
#include <labhelper.h>
#include <chrono>
#include <list>
#include <string>

#include "GameObject.h"

class Engine
{
public:
	SDL_Window* g_window;

	void start();
	bool update();
	void addGameObject(GameObject* o);
	void cleareGameObjects();
	float getDeltaTime();
	float getCurrentTime();
	
private:
	std::chrono::time_point<std::chrono::system_clock> startTime;
	float currentTime;
	float deltaTime;
	bool isUpdating;

	std::list<GameObject*> gameObjects;
};

