#include "Engine.h"



void Engine::start()
{
	g_window = labhelper::init_window_SDL("3D Engine Battle Zone");

	

	startTime = std::chrono::system_clock::now();
	std::chrono::duration<float> timeSinceStart = 
		std::chrono::system_clock::now() - startTime;

	deltaTime = timeSinceStart.count() - currentTime;
	currentTime = timeSinceStart.count();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Load program shaders
	backgroundProgram = labhelper::loadShaderProgram("../lab5-rendertotexture/shaders/background.vert",
		"../lab5-rendertotexture/shaders/background.frag");
	shaderProgram = labhelper::loadShaderProgram("../lab5-rendertotexture/shaders/shading.vert",
		"../lab5-rendertotexture/shaders/shading.frag");
	postFxShader = labhelper::loadShaderProgram("../lab5-rendertotexture/shaders/postFx.vert",
		"../lab5-rendertotexture/shaders/postFx.frag");

	// Load Environment Map
	const std::string envmap_base_name = "001";
	const int roughnesses = 8;
	std::vector<std::string> filenames;
	for (int i = 0; i < roughnesses; i++)
		filenames.push_back("../scenes/envmaps/" + envmap_base_name + "_dl_" + std::to_string(i) + ".hdr");

	reflectionMap = labhelper::loadHdrMipmapTexture(filenames);
	environmentMap = labhelper::loadHdrTexture("../scenes/envmaps/" + envmap_base_name + ".hdr");
	irradianceMap = labhelper::loadHdrTexture("../scenes/envmaps/" + envmap_base_name + "_irradiance.hdr");

	// Set Framebuffers
	int w, h;
	SDL_GetWindowSize(g_window, &w, &h);
	const int numFbos = 5;
	for (int i = 0; i < numFbos; i++) {
		fboList.push_back(FrameBufferObjectInfo(w, h));
	}


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

std::list<GameObject*> Engine::getGameObjects()
{
	return this->gameObjects;
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

void Engine::inputHandle()
{
	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSdlGL3_ProcessEvent(&event);
	}
}
