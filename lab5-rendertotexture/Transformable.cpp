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

	/*charTransform->setTransLate(
		currentPos + wanderVelosity * delta
	);*/

	charRigidBody->velocity = wanderVelosity;



	//count++;

}

RigidBodyComponent::RigidBodyComponent(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
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

	tran->setTransLate(position+velocity* dt);
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
