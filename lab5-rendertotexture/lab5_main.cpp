
/*
this project is based on 
the computer graphics laboratory tutorial 5
at Chalmers University of Technology
http://www.cse.chalmers.se/edu/course/TDA362/tutorials/lab5.html

*/

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
PlayerBehavior* playerBehavior;

GameObject* gameOverObject;


int playerLife = 3;


labhelper::Model* piramid;
labhelper::Model* simpleCube;
labhelper::Model* diamante;
labhelper::Model* tankModel;
labhelper::Model* aimModel;

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
labhelper::Model* gameOverModel = nullptr;
labhelper::Model* ufoModel = nullptr;


void createBullet();

void createPlayer();

void GameOverCheck();

void drawTerrain(const glm::mat4& projection, const glm::mat4& view);

void drawAim(const glm::mat4& view);

void drawPlayerLife(const glm::mat4& view);

void createEnemy(vec2 pos);

void createEnemyBullet(Engine* engine, GameObject* owner, GameObject* target);


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








void loadSceneModels()
{
	terrainModel = labhelper::loadModelFromOBJ("../scenes/terrain.obj");
	tankModel = labhelper::loadModelFromOBJ("../scenes/Tank.obj");
	piramid = labhelper::loadModelFromOBJ("../scenes/pyramid.obj");
	simpleCube = labhelper::loadModelFromOBJ("../scenes/cube2.obj");
	diamante = labhelper::loadModelFromOBJ("../scenes/prism.obj");
	bulletModel = labhelper::loadModelFromOBJ("../scenes/bullet.obj");
	aimModel = labhelper::loadModelFromOBJ("../scenes/aim.obj");
	gameOverModel = labhelper::loadModelFromOBJ("../scenes/gameOver.obj");
	ufoModel = labhelper::loadModelFromOBJ("../scenes/ufo.obj");

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

	vec2 screenSize = engine->getScreenSize();

	drawPlayerLife(view);

	drawAim(view);
}

void drawAim(const glm::mat4& view)
{
	mat4 aimMatrix = scale(vec3(10, 10, 1));
	labhelper::setUniformSlow(
		engine->shaderProgram, "isBilBoard",
		true);

	labhelper::setUniformSlow(
		engine->shaderProgram, "modelViewMatrix",
		view * aimMatrix);

	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", transpose(view * aimMatrix));

	labhelper::setUniformSlow(
		engine->shaderProgram, "offset",
		vec3(0.0f, 0.0f, 0));

	labhelper::render(aimModel);

	labhelper::setUniformSlow(
		engine->shaderProgram, "isBilBoard",
		false);
}

void drawPlayerLife(const glm::mat4& view)
{
	mat4 tankLife = scale(vec3(0.005f, 0.005f, 0.005f));
	labhelper::setUniformSlow(
		engine->shaderProgram, "isBilBoard",
		true);

	labhelper::setUniformSlow(
		engine->shaderProgram, "modelViewMatrix",
		view * tankLife);

	labhelper::setUniformSlow(engine->shaderProgram, "normalMatrix", transpose(view * tankLife));

	for (int i = 0; i < playerBehavior->life; i++) {
		float newOff = 0.9 - 0.15f * i;

		labhelper::setUniformSlow(
			engine->shaderProgram, "offset",
			vec3(newOff, 0.9, 0));

		labhelper::render(tankModel);
	}

	labhelper::setUniformSlow(
		engine->shaderProgram, "isBilBoard",
		false);
}


void display()
{
	vec2 win = engine->getScreenSize();

	for(int i = 0; i < engine->fboList.size(); i++) {
		if(engine->fboList[i].width != win.x || 
			engine->fboList[i].height != win.y)
			engine->fboList[i].resize(win.x, win.y);
	}

	mat4 projectionMatrix = 
		perspective(
			radians(45.0f), 
			float(win.x) / float(win.y), 
			10.0f, 1000.0f
		);
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
	glViewport(0, 0, win.x, win.y);
	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene(viewMatrix, projectionMatrix); // using both shaderProgram and backgroundProgram


	///////////////////////////////////////////////////////////////////////////
	// Post processing pass(es)
	///////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->postFxShader);
	labhelper::setUniformSlow(engine->postFxShader, "time", engine->getCurrentTime());
	labhelper::setUniformSlow(engine->postFxShader, "currentEffect", PostProcessingEffect::None);
	labhelper::setUniformSlow(engine->postFxShader, "filterSize", filterSizes[filterSize - 1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cameraFB.colorTextureTarget);
	labhelper::drawFullScreenQuad();


	glUseProgram(0);

	CHECK_GL_ERROR();
}

void handleEvents(void)
{
	// check events (keyboard among other)
	SDL_Event event;
	
	ImGuiIO& io = ImGui::GetIO();
	while(SDL_PollEvent(&event))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);

		if(event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
		{
			engine->stop();
			return;
		}

		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
			createBullet();
		}
		else if(event.type == SDL_MOUSEBUTTONDOWN) {
			SDL_Log("BOTOOOONNNN %d", event.button.button);
			if (event.button.button == SDL_BUTTON_LEFT)
				createBullet();
		}


		if(event.type == SDL_MOUSEMOTION)
		{
			int delta_x = event.motion.x - g_prevMouseCoords.x;

			if (event.motion.x >= engine->windowsSize.x-5) {
				delta_x = 25;
			}
			if (event.motion.x<=0+5) {
				delta_x = -25;
			}

			float rotationSpeed = 0.1f;
			
			mat4 yaw = rotate(
				rotationSpeed * deltaTime * -delta_x, 
				worldUp
			);
			
				
			cameraDirection = vec3( 
				yaw * vec4(cameraDirection, 0.0f)
			);
			
			
			g_prevMouseCoords.x = event.motion.x;
			g_prevMouseCoords.y = event.motion.y;

			
		}
	}

	if(!io.WantCaptureKeyboard)
	{
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
		
	}
}

void gui()
{
	int test=100;
	bool tt = false;
	int currentTime = engine->getCurrentTime();
	// Inform imgui of new frame
	ImGui_ImplSdlGL3_NewFrame(g_window);

	
	ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f),"SCORE : %d", playerBehavior->score);
	
	if (playerBehavior->life == 3) {
		ImGui::Text("/ ( O w O ) /");
		if (currentTime % 2 == 0)
			ImGui::Text("----____----____----____----____--------____----____----____----____--------____----____----____----____----");
		else
			ImGui::Text("____----____----________----____----________----____----________----____----________----____----________----____----____");

	}
	else if (playerBehavior->life == 2) {
		ImGui::Text("*( > _ < )*");
		if (currentTime % 2 == 0)
			ImGui::Text("--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--");
		else
			ImGui::Text("__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--__--");

	}
	else if (playerBehavior->life == 1) {
		ImGui::Text("m ( ; _ ; ) m");
		if (currentTime % 2 == 0)
			ImGui::Text("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");
		else
			ImGui::Text("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");

	}
	else {
		ImGui::Text("( TT _ TT )");
		
		ImGui::Text("______________________________________________________");
		
	}


	
	
	ImGui::Render();
}

void createEnemy(vec2 pos) {
	GameObject* enemy = new GameObject();

	Collidable* colli = new Collidable(engine, enemy);
	colli->setCollidableRadius(20.0f);

	TankBehavior* behavior = new TankBehavior(engine, enemy);
	behavior->bulletModel = bulletModel;
	behavior->rate = 5;

	Transformable* transformableComp = new Transformable(engine, enemy);
	transformableComp->setRotate(vec3(0.0f, 1.0f, 0.0f), radians(-180.0f));
	transformableComp->setScale(vec3(1, 1.5f, 1));
	transformableComp->setTransLate(vec3(pos.x, 0.0f, pos.y));

	RigidBodyComponent* rigidBody = new RigidBodyComponent(engine, enemy);
	BoxBound* mapBound = new BoxBound(engine, enemy);
	mapBound->SetBounds(
		vec2(1000, 1000),
		vec2(-1000, -1000)
	);
	WandeSeekComponent* wander = new WandeSeekComponent(engine, enemy);
	wander->setTarget(&cameraPosition);
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

void createEnemyBullet(Engine* e, GameObject* owner, GameObject* target)
{

	GameObject* bullet = new GameObject();
	BulletBehavior* behavior = new BulletBehavior(e, bullet);
	behavior->SetOwner(owner);

	Renderable* renderBullet = new Renderable(e, bullet);

	renderBullet->setModel(bulletModel);

	Transformable* transf = new Transformable(e, bullet);
	Transformable* ownTransf = (Transformable*)owner->getComponent(TRANSFORMABLE);

	vec3 ownerPos = ownTransf->getTranslate();
	vec3 bPos = vec3();

	bPos.y = ownerPos.y;
	bPos.z = ownerPos.z;
	bPos.x = ownerPos.x;


	transf->setTransLate(bPos);

	RigidBodyComponent* rigidBody = new RigidBodyComponent(e, bullet);

	Collidable* colli = new Collidable(e, bullet);
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

	e->addGameObject(bullet);
}

void createSceneObjects() {
	// hasta <=2 original
	for (int i=-2; i <= 2; i++) {
		GameObject* enemy = new GameObject();

		Collidable* colli = new Collidable(engine, enemy);
		colli->setCollidableRadius(20.0f);

		TankBehavior* behavior = new TankBehavior(engine, enemy);
		behavior->addFireFunction(&createEnemyBullet);
		behavior->bulletModel = bulletModel;
		behavior->rate = (i + 3)*10;

		Transformable* transformableComp = new Transformable(engine, enemy);
		transformableComp->setRotate(vec3(0.0f, 1.0f, 0.0f), radians(-180.0f));
		transformableComp->setScale(vec3(1, 1.5f, 1));
		if(i == 0)
			transformableComp->setTransLate(vec3(200.0f, 0.0f, 200.0f));
		else
			transformableComp->setTransLate(vec3(i*70.0f, 0.0f, i*70.0f));

		RigidBodyComponent* rigidBody = new RigidBodyComponent(engine, enemy);
		BoxBound* mapBound = new BoxBound(engine, enemy);
		mapBound->SetBounds(
			vec2(700,700), 
			vec2(-700, -700)
		);
		//WanderingComponent* wander = new WanderingComponent(engine, enemy);
		WandeSeekComponent* wander = new WandeSeekComponent(engine, enemy);
		wander->setTarget(&cameraPosition);
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


	// int i = -10; i <= 10; i++
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


	//createEnemy(vec2(50, -50));


	GameObject* UFO = new GameObject();
	Renderable* ufoRender = new Renderable(engine, UFO);
	ufoRender->setModel(ufoModel);

	RigidBodyComponent* ufoRigidBody =
		new RigidBodyComponent(engine, UFO);

	Transformable* ufoTransformable = 
		new Transformable(engine, UFO);
	ufoTransformable->setTransLate(
		vec3(-350,0,10)
	);

	UfoBehavior* ufoBehavior =
		new UfoBehavior(engine, UFO);
	vec3 setofpoints[] = {
		vec3(-300.0f,0.0f,-10.0f),
		vec3(-30.0f,0.0f,20.0f),
		vec3(10.0f,0.0f,-30.0f),
		vec3(20.0f,0.0f,40.0f),
		vec3(100.0f,0.0f,-20.0f),

		vec3(120.0f,0.0f,-200.0f),
		vec3(40.0f,0.0f,-100.0f),
		vec3(0.0f,0.0f,-300.0f),
		vec3(-200.0f,0.0f,-200.0f)
	};

	ufoBehavior->maxVelosity = 50;
	ufoBehavior->setPoints(setofpoints);
	ufoBehavior->setTransform(ufoTransformable);
	ufoBehavior->setRigidBody(ufoRigidBody);

	UFO->addComponent(
		ufoBehavior, BEHAVIOR
	);

	UFO->addComponent(
		ufoTransformable, TRANSFORMABLE
	);

	UFO->addComponent(
		ufoRender, RENDERABLE
	);

	UFO->addComponent(
		ufoRigidBody, RIGID_BODY
	);

	engine->addGameObject(UFO);
	
}

void createBullet() {

	GameObject* bullet = new GameObject();
	BulletBehavior* behavior = new BulletBehavior(engine, bullet);
	behavior->SetOwner(playerObject);

	Renderable* renderBullet = new Renderable(engine, bullet);

	renderBullet->setModel(bulletModel);

	Transformable* transf = new Transformable(engine, bullet);
	
	vec3 bPos = vec3();

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

	engine->addGameObject(bullet);
}

void createPlayer()
{
	playerObject = new GameObject();
	playerBehavior =
		new PlayerBehavior(engine, playerObject);
	playerBehavior->life = 3;
	Transformable* trsp =
		new Transformable(engine, playerObject);
	trsp->setTransLate(cameraPosition);

	RigidBodyComponent* rig = new RigidBodyComponent(engine, playerObject);
	rig->velocity = vec3(cameraDirection.x, 0, cameraDirection.z);

	Collidable* coll = new Collidable(engine, playerObject);
	coll->setCollidableRadius(20.0f);

	playerObject->addComponent(
		playerBehavior, BEHAVIOR
	);
	playerObject->addComponent(
		trsp, TRANSFORMABLE
	);
	playerObject->addComponent(
		rig, RIGID_BODY
	);

	playerObject->addComponent(
		coll, COLLIDABLE
	);


	engine->addGameObject(playerObject);
}

void GameOverCheck()
{
	if (playerBehavior->life <= 0 && gameOverObject == nullptr) {
		cameraPosition = vec3(0.0f, 0.0f, 70.0f);
		cameraDirection = normalize(vec3(0.0f) - cameraPosition);
		engine->deleteObjectsByBehavior<TankBehavior*>(BEHAVIOR);

		gameOverObject = new GameObject();

		Transformable* gameOverTransf = new Transformable(engine, gameOverObject);
		gameOverTransf->setTransLate(vec3(0.0f, 10.0f, 0.0f));

		Renderable* gameOverRender = new Renderable(engine, gameOverObject);
		gameOverRender->setModel(gameOverModel);

		gameOverObject->addComponent(gameOverRender, RENDERABLE);
		gameOverObject->addComponent(gameOverTransf, TRANSFORMABLE);

		engine->addGameObject(gameOverObject);
	}
}

int main(int argc, char* argv[])
{
	engine = new Engine();
	engine->start();

	g_window = engine->g_window;
	loadSceneModels();

	createPlayer();
	
	createSceneObjects();


	
	while(engine->update())
	{
		deltaTime = engine->getDeltaTime();
		currentTime = engine->getCurrentTime();

		display();

		gui();
		

		handleEvents();
		
		Transformable* playerTrans =
			(Transformable*)playerObject->getComponent(TRANSFORMABLE);
		playerTrans->setTransLate(cameraPosition);

		RigidBodyComponent* rig =
			(RigidBodyComponent*)playerObject->getComponent(RIGID_BODY);

		rig->velocity = vec3(cameraDirection.x, 0, cameraDirection.z);


		
		GameOverCheck();

		if (engine->getGameObjects().size() < 10) {
			float posX = rand() % 500 + 1;
			float posY = rand() % 500 + 1;

			vec2 newPosEnemy(posX, posY);
			createEnemy(newPosEnemy);
		}
	}

	// Free Models
	labhelper::freeModel(landingpadModel);
	labhelper::freeModel(fighterModel);
	labhelper::freeModel(sphereModel);

	labhelper::shutDown(engine->g_window);
	return 0;
}


