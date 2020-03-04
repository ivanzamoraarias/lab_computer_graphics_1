#pragma once

#include <GL/glew.h>
#include <labhelper.h>
#include <chrono>
#include <list>
#include <string>
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <Model.h>
#include "hdr.h"
#include "GameObject.h"
#include <glm/glm.hpp>
using namespace glm;


struct FrameBufferObjectInfo
{
	GLuint framebufferId;
	GLuint colorTextureTarget;
	GLuint depthBuffer;
	int width;
	int height;
	bool isComplete;

	FrameBufferObjectInfo(int w, int h)
	{
		isComplete = false;
		width = w;
		height = h;
		// Generate two textures and set filter parameters (no storage allocated yet)
		glGenTextures(1, &colorTextureTarget);
		glBindTexture(GL_TEXTURE_2D, colorTextureTarget);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &depthBuffer);
		glBindTexture(GL_TEXTURE_2D, depthBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// allocate storage for textures
		resize(width, height);

		///////////////////////////////////////////////////////////////////////
		// Generate and bind framebuffer
		///////////////////////////////////////////////////////////////////////
		// >>> @task 1
		glGenFramebuffers(1, &framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
		// bind the texture as color attachment 0 (to the currently bound framebuffer)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureTarget, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		// bind the texture as depth attachment (to the currently bound framebuffer)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

		// check if framebuffer is complete
		isComplete = checkFramebufferComplete();

		// bind default framebuffer, just in case.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// if no resolution provided
	FrameBufferObjectInfo()
		: isComplete(false)
		, framebufferId(UINT32_MAX)
		, colorTextureTarget(UINT32_MAX)
		, depthBuffer(UINT32_MAX)
		, width(0)
		, height(0) {};

	void resize(int w, int h)
	{
		width = w;
		height = h;
		// Allocate a texture
		glBindTexture(GL_TEXTURE_2D, colorTextureTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// generate a depth texture
		glBindTexture(GL_TEXTURE_2D, depthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
			nullptr);
	}

	bool checkFramebufferComplete(void)
	{
		// Check that our FBO is correctly set up, this can fail if we have
		// incompatible formats in a buffer, or for example if we specify an
		// invalid drawbuffer, among things.
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			labhelper::fatal_error("Framebuffer not complete");
		}

		return (status == GL_FRAMEBUFFER_COMPLETE);
	}
};

class Engine
{
public:
	//bool isUpdating;

	mat4 projectionMatrix;
	mat4 viewMatrix;

	GLuint backgroundProgram, shaderProgram, postFxShader;

	GLuint environmentMap, irradianceMap, reflectionMap;

	std::vector<FrameBufferObjectInfo> fboList;

	SDL_Window* g_window;

	void start();
	bool update();
	void addGameObject(GameObject* o);
	std::list<GameObject*> getGameObjects();
	void cleareGameObjects();
	float getDeltaTime();
	float getCurrentTime();
	void inputHandle();
	vec2 getScreenSize();

	template<typename Base, typename T>
	static bool instanceof(const T*);
	
private:
	std::chrono::time_point<std::chrono::system_clock> startTime;
	float currentTime;
	float deltaTime;
	bool isUpdating;

	

	std::list<GameObject*> gameObjects;
};

template<typename Base, typename T>
inline bool Engine::instanceof(const T*)
{
	bool res = std::is_base_of<Base, T>::value;

	return std::is_base_of<Base, T>::value;
}
