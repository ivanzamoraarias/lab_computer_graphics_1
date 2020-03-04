#pragma once
#include "Transformable.h"
#include "Engine.h"


void Transformable::update()
{
	//std::cout << "Upddate Game Entity" << std::endl;

}

void Transformable::update(GameEntity& object) {
	//std::cout << "Upddate Game Entity" << std::endl;
}

vec3 Transformable::getTranslate()
{
	return this->translateVector;
}

void Transformable::setTransLate(vec3 translate)
{
	this->translateVector = translate;
}

vec3 Transformable::getScale()
{
	return this->scaleVector;
}

void Transformable::setScale(vec3 scale)
{
	this->scaleVector = scale;
}

vec3 Transformable::getRotate()
{
	return this->rotateVector;
}

float Transformable::getAngle()
{
	return this->rotateAngle;
}

void Transformable::setRotate(vec3 rotate, float angle)
{
	this->rotateVector = rotate;
	this->rotateAngle = angle;
}

void Transformable::setRotateAngle(float angle)
{
	this->rotateAngle = angle;
}

mat4 Transformable::getTransformationMatrix()
{
	mat4 resultingMatrix =
		translate(translateVector) *
		rotate(rotateAngle, rotateVector) *
		scale(scaleVector);


	return resultingMatrix;
}

Transformable::Transformable(Engine* e, GameObject* go)
{
	this->rotateAngle = 0.0f;
	this->translateVector = vec3(1, 1, 1);
	this->scaleVector = vec3(1, 1, 1);
	this->rotateAngle = 0;
	this->rotateVector = vec3(0, 1, 0);

	ObjectComponent::create(e, go);
}

Transformable::~Transformable()
{
}

Renderable::Renderable(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void Renderable::update()
{

	Transformable* myTransform = (Transformable*)this
		->gameObject
		->getComponent(componentType::TRANSFORMABLE);

	if (myTransform == nullptr)
		return;

	mat4 myMatrix = myTransform->getTransformationMatrix();

	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"modelViewProjectionMatrix", 
		engine->projectionMatrix * engine->viewMatrix * myMatrix
	);
	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"modelViewMatrix", 
		engine->viewMatrix * myMatrix
	);

	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"normalMatrix", 
		inverse(
			transpose(engine->viewMatrix * myMatrix)
		)
	);

	labhelper::render(this->model);
}

void Renderable::setModel(labhelper::Model* m)
{
	this->model = m;
}

WanderingComponent::WanderingComponent(Engine* e, GameObject* go)
{
	wanderVelosity = vec3(20.0f, 0, 0);
	ObjectComponent::create(e, go);
}

void WanderingComponent::update()
{
	int cTime = this->engine->getCurrentTime();

	RigidBodyComponent* charRigidBody = (RigidBodyComponent*)this
		->gameObject
		->getComponent(componentType::RIGID_BODY);

	Transformable* charTransform = (Transformable*)this
		->gameObject
		->getComponent(componentType::TRANSFORMABLE);

	vec3 currentPos = charTransform->getTranslate();
	vec3 unitatio = normalize(currentPos);
	float delta = (float)this->engine->getDeltaTime();

	if (cTime % 5 == 0 && count != cTime) {
		count = cTime;
		std::cout << "on: " << cTime << std::endl;
		float newRotation = rand() % (int)maxRotation + 1;

		vec2 charOrientation(
			unitatio.x * maxSpeed,
			unitatio.z * maxSpeed
		);

		charOrientation = rotate(charOrientation, newRotation);

		wanderVelosity = vec3(
			charOrientation.x,
			0,
			charOrientation.y
		);

		charTransform->setRotateAngle(newRotation);
	}

	charRigidBody->velocity = wanderVelosity;



	//count++;

}

RigidBodyComponent::RigidBodyComponent(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	velocity = vec3(0,0,0);
	acceleration = vec3(0, 0, 0);
}

void RigidBodyComponent::update()
{
	Transformable* tran =
		(Transformable*)this->gameObject
		->getComponent(componentType::TRANSFORMABLE);
	
	if (!tran)
		return;
	vec3 position = tran->getTranslate();
	float dt = engine->getDeltaTime();

	if(acceleration.x == 0 && acceleration.y == 0 && acceleration.z== 0)
		tran->setTransLate(position+velocity * dt);
	else 
		tran->setTransLate(position + velocity + acceleration * dt);
	//go->position = go->position + velocity * dt;
}

BoxBound::BoxBound(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void BoxBound::SetBounds(vec2 front, vec2 back)
{
	this->front = front;
	this->back = back;
}

void BoxBound::update()
{
	Transformable* transf =
		(Transformable*)this->gameObject
		->getComponent(TRANSFORMABLE);

	RigidBodyComponent* rigidBody =
		(RigidBodyComponent*)this->gameObject
		->getComponent(RIGID_BODY);

	vec3 pos = transf->getTranslate();
	vec2 twod(pos.x,pos.z);
	vec2 velosity = rigidBody->velocity;
	float delta = engine->getDeltaTime();
	if (((twod.x + velosity.x * delta) >= this->front.x) ||
		((twod.y + velosity.y * delta) >= this->front.y) ||
		((twod.x + velosity.x * delta) <= this->back.x) ||
		((twod.y + velosity.y * delta) <= this->back.y) ) {
		rigidBody->velocity *= -1.0f;
	}
}

bool BulletBehavior::isOutOfRange()
{
	Transformable* myTra =
		(Transformable*)this->gameObject->getComponent(TRANSFORMABLE);

	vec3 currentPos = myTra->getTranslate();

	double d = distance(currentPos, initialPosition);

	return (d > 500.0f);
}

BulletBehavior::BulletBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void BulletBehavior::SetOwner(GameObject* o)
{
	this->owner = o;
}

void BulletBehavior::Start()
{
	RigidBodyComponent* ownerBody =
		(RigidBodyComponent*)this->owner->getComponent(RIGID_BODY);
	RigidBodyComponent* myBody =
		(RigidBodyComponent*)this->gameObject->getComponent(RIGID_BODY);
	Transformable* ownerTra =
		(Transformable*)this->owner->getComponent(TRANSFORMABLE);

	this->initialPosition = ownerTra->getTranslate();

	vec3 uniVelosity = normalize(ownerBody->velocity);

	myBody->velocity = uniVelosity;
	myBody->acceleration = uniVelosity;
}

void BulletBehavior::update()
{

	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);
	
	bool isCollided = myColli->isCollided;

	bool outOfRange = this->isOutOfRange();

	if (outOfRange || isCollided) {
		SDL_Log("DESTROY BULLET");
		this->gameObject->Destroy();
	}

}

Collidable::Collidable(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	this->isCollided = false;
}

void Collidable::update()
{
	Transformable* tr = (Transformable*)this->gameObject->getComponent(TRANSFORMABLE);
	geometry.x = tr->getTranslate().x;
	geometry.y = tr->getTranslate().z;

	this->currentColided = nullptr;
	this->isCollided = false;

	for (GameObject* object : engine->getGameObjects()) {
		if (object == this->gameObject)
			continue;

		Collidable* other =
			(Collidable*)object->getComponent(COLLIDABLE);

		if (!other)
			continue;

		CollidableGeometry g1 = this->geometry;
		CollidableGeometry g2 = other->geometry;

		float dx = g1.x - g2.x;
		float dy = g1.y - g2.y;

		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance < (g1.radius + g2.radius)) {
			other->isCollided = 1;
			this->isCollided = 1;
			this->currentColided = object;
			other->currentColided = this->gameObject;

			return;
		}
	}
	
	//this->currentColided = nullptr;
}

bool Collidable::isObjectCollided()
{
	return isCollided;
}

void Collidable::setCollidableRadius(double r)
{
	this->geometry.radius = r;
}

bool Collidable::isOwner(GameObject* o)
{
	return false;
}

RockBehavior::RockBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void RockBehavior::update()
{
	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);

	bool isCollided = myColli->isCollided;
	
	if (!isCollided || !myColli->currentColided)
		return;

	ObjectComponent* behavior =
		myColli->currentColided->getComponent(BEHAVIOR);

	BulletBehavior* t = dynamic_cast<BulletBehavior*>(behavior);


	if ( t != nullptr) {

		SDL_Log(":V YES");
		this->gameObject->Destroy();

	}
}

TankBehavior::TankBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void TankBehavior::update()
{
	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);

	bool isCollided = myColli->isCollided;

	if (!isCollided || !myColli->currentColided)
		return;

	ObjectComponent* behavior =
		myColli->currentColided->getComponent(BEHAVIOR);

	BulletBehavior* t = dynamic_cast<BulletBehavior*>(behavior);

	RockBehavior* rock = dynamic_cast<RockBehavior*>(behavior);


	if (t != nullptr) {

		SDL_Log(":V YES");
		this->gameObject->Destroy();

	}

	if (rock != nullptr) {
		RigidBodyComponent* rigidBody =
			(RigidBodyComponent*)this->gameObject->getComponent(RIGID_BODY);
		
		rigidBody->velocity *= -1.0f;
	}
}
