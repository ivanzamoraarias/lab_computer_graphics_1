
#include <algorithm>
#include <iostream>
#include "Engine.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;


#include "Transformable.h"
#include <memory>

using std::min;
using std::max;



Engine* engine;
GameObject* playerObject;


labhelper::Model* piramid;
labhelper::Model* simpleCube;
labhelper::Model* diamante;
labhelper::Model* tankModel;

SDL_Window* g_window = nullptr;
static float currentTime = 0.0f;
static float deltaTime = 0.0f;
bool showUI = false;

// Mouse input
ivec2 g_prevMouseCoords = { -1, -1 };
bool g_isMouseDragging = false;

//GLuint backgroundProgram, shaderProgram, postFxShader;

float environment_multiplier = 1.0f;
//GLuint environmentMap, irradianceMap, reflectionMap;
const std::string envmap_base_name = "001";

float point_light_intensity_multiplier = 2000.0f;
vec3 point_light_color = vec3(1.f, 1.f, 1.f);
const vec3 lightPosition = vec3(20.0f, 40.0f, 0.0f);

vec3 cameraPosition(-70.0f, 0.0f, 70.0f);
vec3 cameraDirection = normalize(vec3(0.0f) - cameraPosition);
float cameraSpeed = 10.f;

vec3 worldUp(0.0f, 1.0f, 0.0f);

labhelper::Model* landingpadModel = nullptr;
labhelper::Model* fighterModel = nullptr;
labhelper::Model* enemyModel = nullptr;
labhelper::Model* sphereModel = nullptr;
labhelper::Model* terrainModel = nullptr;
labhelper::Model* bulletModel = nullptr;


void createBullet();

void drawTerrain(const glm::mat4& projection, const glm::mat4& view);


enum PostProcessingEffect
{
	None = 0,
	Sepia = 1,
	Mushroom = 2,
	Blur = 3,
	Grayscale = 4,
	Composition = 5,
	Mosaic = 6,
	Separable_blur = 7,
	Bloom = 8
};

int currentEffect = PostProcessingEffect::None;
int filterSize = 1;
int filterSizes[12] = { 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25 };


struct FrameBufferObjectInfo;
std::vector<FrameBufferObjectInfo> fboList;

vec3 movePlayer(0.0f,0.0f,0.0f);
vec3 rotatePlayer(0.0f, 0.0f, 1.0f);
float rotatePlayerAngle = 0.0f;




void loadSceneModels()
{
	terrainModel = labhelper::loadModelFromOBJ("../scenes/terrain.obj");
	tankModel = labhelper::loadModelFromOBJ("../scenes/Tank.obj");
	piramid = labhelper::loadModelFromOBJ("../scenes/pyramid.obj");
	simpleCube = labhelper::loadModelFromOBJ("../scenes/cube2.obj");
	diamante = labhelper::loadModelFromOBJ("../scenes/prism.obj");
	bulletModel = labhelper::loadModelFromOBJ("../scenes/bullet.obj");
}

void drawTerrain(const glm::mat4& projection, const glm::mat4& view)
{
	mat4 terrainModelMatrix(1.0f);
	vec3 translateTerrain(0.0f, -13.0f, 0.0f);
	vec3 scaleTerrain(2.0f, 2.0f, 2.0f);

	terrainModelMatrix = translate(translateTerrain) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(400.0f, -13.0f, 0.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(-400.0f, -13.0f, 0.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(0.0f, -13.0f, 400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(0.0f, -13.0f, -400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(-400.0f, -13.0f, -400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(400.0f, -13.0f, -400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);


	terrainModelMatrix = translate(vec3(-400.0f, -13.0f, 400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);

	terrainModelMatrix = translate(vec3(400.0f, -13.0f, 400.0f)) * scale(scaleTerrain);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewProjectionMatrix", projection * view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "modelViewMatrix", view * terrainModelMatrix);
	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", inverse(transpose(view * terrainModelMatrix)));

	labhelper::render(terrainModel);
}

void drawScene(const mat4& view, const mat4& projection)
{
	glUseProgram(engine->backgroundProgram);
	labhelper::setUniformSlow(engine->backgroundProgram, "environment_multiplier", environment_multiplier);
	labhelper::setUniformSlow(engine->backgroundProgram, "inv_PV", inverse(projection * view));
	labhelper::setUniformSlow(engine->backgroundProgram, "camera_pos", cameraPosition);
	labhelper::drawFullScreenQuad();

	glUseProgram(engine->shaderProgram);
	// Light source
	vec4 viewSpaceLightPosition = view * vec4(lightPosition, 1.0f);
	labhelper::setUniformSlow(engine->shaderProgram, "point_light_color", point_light_color);
	labhelper::setUniformSlow(engine->shaderProgram, "point_light_intensity_multiplier",
	                          point_light_intensity_multiplier);
	labhelper::setUniformSlow(engine->shaderProgram, "viewSpaceLightPosition", vec3(viewSpaceLightPosition));

	// Environment
	labhelper::setUniformSlow(engine->shaderProgram, "environment_multiplier", environment_multiplier);

	// camera
	labhelper::setUniformSlow(engine->shaderProgram, "viewInverse", inverse(view));


	// terrainModel

	drawTerrain(projection, view);

	for (GameObject* g: engine->getGameObjects()) {
		Renderable* re = (Renderable*)g->getComponent(componentType::RENDERABLE);
		if (re == nullptr)
			continue;
		re->update();
	}

}


void display()
{
	int w, h;
	SDL_GetWindowSize(g_window, &w, &h);

	for(int i = 0; i < engine->fboList.size(); i++)
	{
		if(engine->fboList[i].width != w || engine->fboList[i].height != h)
			engine->fboList[i].resize(w, h);
	}

	mat4 projectionMatrix = perspective(radians(45.0f), float(w) / float(h), 10.0f, 1000.0f);
	mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraDirection, worldUp);

	engine->projectionMatrix = projectionMatrix;
	engine->viewMatrix = viewMatrix;

	///////////////////////////////////////////////////////////////////////////
	// Bind the environment map(s) to unused texture units
	///////////////////////////////////////////////////////////////////////////
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, engine->environmentMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, engine->irradianceMap);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, engine->reflectionMap);
	

	///////////////////////////////////////////////////////////////////////////
	// draw scene from camera
	///////////////////////////////////////////////////////////////////////////

	

	FrameBufferObjectInfo& cameraFB = engine->fboList[1];
	glBindFramebuffer(GL_FRAMEBUFFER, cameraFB.framebufferId); // to be replaced with another framebuffer when doing post processing
	glViewport(0, 0, w, h);
	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene(viewMatrix, projectionMatrix); // using both shaderProgram and backgroundProgram


	///////////////////////////////////////////////////////////////////////////
	// Post processing pass(es)
	///////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->postFxShader);
	labhelper::setUniformSlow(engine->postFxShader, "time", currentTime);
	labhelper::setUniformSlow(engine->postFxShader, "currentEffect", currentEffect);
	labhelper::setUniformSlow(engine->postFxShader, "filterSize", filterSizes[filterSize - 1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cameraFB.colorTextureTarget);
	labhelper::drawFullScreenQuad();


	glUseProgram(0);

	CHECK_GL_ERROR();
}

bool handleEvents(void)
{
	// check events (keyboard among other)
	SDL_Event event;
	bool quitEvent = false;
	ImGuiIO& io = ImGui::GetIO();
	while(SDL_PollEvent(&event))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);

		if(event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
		{
			quitEvent = true;
		}
		if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_g)
		{
			showUI = !showUI;
		}

		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
			createBullet();
		}
	
		else if(event.type == SDL_MOUSEBUTTONDOWN
		        && (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
		        && (!showUI || !ImGui::GetIO().WantCaptureMouse))
		{
			g_isMouseDragging = true;
			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			g_prevMouseCoords.x = x;
			g_prevMouseCoords.y = y;
		}

		uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
		if(!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && !(mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)))
		{
			g_isMouseDragging = false;
		}

		if(event.type == SDL_MOUSEMOTION && g_isMouseDragging)
		{
			// More info at https://wiki.libsdl.org/SDL_MouseMotionEvent
			int delta_x = event.motion.x - g_prevMouseCoords.x;
			int delta_y = event.motion.y - g_prevMouseCoords.y;
			float rotationSpeed = 0.1f;
			if(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				mat4 yaw = rotate(rotationSpeed * deltaTime * -delta_x, worldUp);
				mat4 pitch = rotate(rotationSpeed * deltaTime * -delta_y,
				                    normalize(cross(cameraDirection, worldUp)));
				//cameraDirection = vec3(pitch * yaw * vec4(cameraDirection, 0.0f));
				cameraDirection = vec3( yaw * vec4(cameraDirection, 0.0f));
			}
			
			g_prevMouseCoords.x = event.motion.x;
			g_prevMouseCoords.y = event.motion.y;
		}
	}

	if(!io.WantCaptureKeyboard)
	{
		// check keyboard state (which keys are still pressed)
		const uint8_t* state = SDL_GetKeyboardState(nullptr);
		vec3 cameraRight = cross(cameraDirection, worldUp);
		if(state[SDL_SCANCODE_W])
		{
			cameraPosition += deltaTime * cameraSpeed * cameraDirection;
		}
		if(state[SDL_SCANCODE_S])
		{
			cameraPosition -= deltaTime * cameraSpeed * cameraDirection;
		}
		if(state[SDL_SCANCODE_A])
		{
			cameraPosition -= deltaTime * cameraSpeed * cameraRight;
		}
		if(state[SDL_SCANCODE_D])
		{
			cameraPosition += deltaTime * cameraSpeed * cameraRight;
		}
		if(state[SDL_SCANCODE_SPACE]){
			
			
		}
		
		if (state[SDL_SCANCODE_RIGHT]) {
			rotatePlayerAngle -= currentTime*0.001f;
			rotatePlayer = vec3(0.0f,1.0f,0.0f);
		}
		if (state[SDL_SCANCODE_LEFT]) {
			rotatePlayerAngle += currentTime*0.001f;
			rotatePlayer = vec3(0.0f, 1.0f, 0.0f);
		}
		if (state[SDL_SCANCODE_UP]) {
			movePlayer += vec3(0.01f * currentTime, 0.0f, 0.0f);
		}
		if (state[SDL_SCANCODE_DOWN]) {
			movePlayer += vec3(-0.01f*currentTime, 0.0f, 0.0f);
		}
	}

	return quitEvent;
}

void gui()
{
	// Inform imgui of new frame
	ImGui_ImplSdlGL3_NewFrame(g_window);

	ImGui::Render();
}



void createEnemy() {

	for (int i=-2; i <= 2; i++) {
		GameObject* enemy = new GameObject();

		Collidable* colli = new Collidable(engine, enemy);
		colli->setCollidableRadius(20.0f);

		TankBehavior* behavior = new TankBehavior(engine, enemy);

		Transformable* transformableComp = new Transformable(engine, enemy);
		transformableComp->setRotate(vec3(0.0f, 1.0f, 0.0f), float(M_PI) / 2.0f);
		transformableComp->setScale(vec3(1, 1.5f, 1));
		transformableComp->setTransLate(vec3(i*70.0f, 0.0f, i*70.0f));

		RigidBodyComponent* rigidBody = new RigidBodyComponent(engine, enemy);
		BoxBound* mapBound = new BoxBound(engine, enemy);
		mapBound->SetBounds(
			vec2(1000,1000), 
			vec2(-1000, -1000)
		);
		WanderingComponent* wander = new WanderingComponent(engine, enemy);
		Renderable* tankRenderable = new Renderable(engine, enemy);
		tankRenderable->setModel(tankModel);

		enemy->addComponent(
			rigidBody, RIGID_BODY
		);
		enemy->addComponent(
			transformableComp, TRANSFORMABLE
		);
		enemy->addComponent(
			wander, AI
		);
		enemy->addComponent(
			tankRenderable, RENDERABLE
		);
		enemy->addComponent(
			mapBound, BOUND
		);

		enemy->addComponent(
			behavior, BEHAVIOR
		);

		enemy->addComponent(
			colli, COLLIDABLE
		);


		engine->addGameObject(enemy);
	}


	
	for (int i = -10; i <= 10; i++) {
		int randNum = rand() % 100;
		GameObject* sceneThing = new GameObject();


		RockBehavior* behavior = new RockBehavior(engine, sceneThing);

		Transformable* transformableComp = new Transformable(engine, sceneThing);
		transformableComp->setRotate(vec3(0.0f, 1.0f, 0.0f), float(M_PI) / 2.0f);
		transformableComp->setScale(
			vec3(5, 5, 5)
		);
		transformableComp->setTransLate(
			vec3(
				i * 70.0f, 
				-4.0f, 
				(200.0f) *sin(i * 70.0f)
			)
		);
		
		Collidable* colli = new Collidable(engine, sceneThing);
		colli->setCollidableRadius(20.0f);

		Renderable* tankRenderable = new Renderable(engine, sceneThing);
		if(randNum%2==0)
			tankRenderable->setModel(piramid);
		else if(randNum%3==0)
			tankRenderable->setModel(simpleCube);
		else 
			tankRenderable->setModel(diamante);

		sceneThing->addComponent(
			transformableComp, TRANSFORMABLE
		);
		sceneThing->addComponent(
			colli, COLLIDABLE
		);
		sceneThing->addComponent(
			tankRenderable, RENDERABLE
		);
		sceneThing->addComponent(
			behavior, BEHAVIOR
		);

		engine->addGameObject(sceneThing);
	}


	

	
}

void createBullet() {
	
	Transformable* playerTrans =
		(Transformable*)playerObject->getComponent(TRANSFORMABLE);
	playerTrans->setTransLate(cameraPosition);

	RigidBodyComponent* rig = 
		(RigidBodyComponent*)playerObject->getComponent(RIGID_BODY);

	rig->velocity = vec3(cameraDirection.x, 0, cameraDirection.z);

	GameObject* bullet = new GameObject();
	BulletBehavior* behavior = new BulletBehavior(engine, bullet);
	behavior->SetOwner(playerObject);

	Renderable* renderBullet = new Renderable(engine, bullet);

	renderBullet->setModel(bulletModel);

	Transformable* transf = new Transformable(engine, bullet);
	
	vec3 bPos = vec3();
	
	//bPos.y = cameraPosition.y - 11;
	//bPos.z = cameraPosition.z + 55;
	//bPos.x = cameraPosition.x + 17;

	bPos.y = cameraPosition.y;
	bPos.z = cameraPosition.z;
	bPos.x = cameraPosition.x;


	transf->setTransLate(bPos);
	 
	RigidBodyComponent* rigidBody = new RigidBodyComponent(engine, bullet);
	
	Collidable* colli = new Collidable(engine, bullet);
	colli->setCollidableRadius(10.0f);

	bullet->addComponent(
		behavior, BEHAVIOR
	);
	bullet->addComponent(
		renderBullet, RENDERABLE
	);
	bullet->addComponent(
		transf, TRANSFORMABLE
	);
	bullet->addComponent(
		rigidBody, RIGID_BODY
	);
	bullet->addComponent(
		colli, COLLIDABLE
	);

	behavior->Start();

	SDL_Log("Adding Bullet");

	engine->addGameObject(bullet);
}

int main(int argc, char* argv[])
{
	playerObject = new GameObject();
	Transformable* trsp = new Transformable(engine, playerObject);
	trsp->setTransLate(cameraPosition);

	RigidBodyComponent* rig = new RigidBodyComponent(engine, playerObject);
	rig->velocity = vec3(cameraDirection.x,0, cameraDirection.z);

	playerObject->addComponent(
		trsp, TRANSFORMABLE
	);
	playerObject->addComponent(
		rig, RIGID_BODY
	);

	engine = new Engine();

	engine->start();
	g_window = engine->g_window;
	loadSceneModels();
	
	//createTank();
	createEnemy();
	
	bool stopRendering = false;

	while(engine->update())
	{
		deltaTime = engine->getDeltaTime();
		currentTime = engine->getCurrentTime();

		display();

		if(showUI)
		{
			gui();
		}

		stopRendering = handleEvents();
	}

	// Free Models
	labhelper::freeModel(landingpadModel);
	labhelper::freeModel(fighterModel);
	labhelper::freeModel(sphereModel);

	labhelper::shutDown(engine->g_window);
	return 0;
}


