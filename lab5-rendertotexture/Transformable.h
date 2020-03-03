#pragma once
#include "ObjectComponent.h"
#include "GameObject.h">
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
#include <iostream>
#include <Model.h>

 

class Transformable: public ObjectComponent
{
private:
	vec3 translateVector;
	vec3 scaleVector;
	vec3 rotateVector;
	float rotateAngle;
public:
	Transformable(Engine* e, GameObject* go);
	~Transformable();
	void update();
    void update(GameEntity& object);
	vec3 getTranslate();
	void setTransLate(vec3 translate);
	vec3 getScale();
	void setScale(vec3 scale);
	vec3 getRotate();
	float getAngle();
	void setRotate(vec3 rotate, float angle);
	void setRotateAngle(float angle);

	mat4 getTransformationMatrix();

   
};

class RigidBodyComponent : public ObjectComponent
{
public:
	vec3 velocity, acceleration;
	RigidBodyComponent(Engine* e, GameObject* go);
	virtual void update();
};


class Renderable : public ObjectComponent {
private:
	labhelper::Model* model;
public:
	Renderable(Engine* e, GameObject* go);
	virtual void update();
	void setModel(labhelper::Model* m);

};

class WanderingComponent : public ObjectComponent {
private:
	vec3 wanderVelosity;
	float wanderRate;
	float wanderOrientation;
	float maxSpeed=25.5f;
	int maxRotation=180;
	int count;
public:
	WanderingComponent(Engine* e, GameObject* go);

	virtual void update();
};

class WandeSeekComponent : public ObjectComponent {
private:

};

class BoxBound : public ObjectComponent {
private:
	vec2 front;
	vec2 back;
public:
	BoxBound(Engine* e, GameObject* go);
	
	void SetBounds(vec2 front, vec2 back);

	virtual void update();
};

class BulletBehavior : public ObjectComponent {
private:
	GameObject* owner;
	vec3 initialPosition;

	bool isOutOfRange();

public:
	BulletBehavior(Engine* e, GameObject* go);

	void SetOwner(GameObject* o);

	void Start();

	virtual void update();
};

struct CollidableGeometry {
	double radius;
	double x;
	double y;
};

class Collidable : public ObjectComponent {
private:
	CollidableGeometry geometry;
public: 
	bool isCollided;
	GameObject* currentColided;

	Collidable(Engine* e, GameObject* go);
	virtual void update();
	bool isObjectCollided();
	void setCollidableRadius(double r);
	bool isOwner(GameObject* o);
};

class RockBehavior : public ObjectComponent {
private:

public:
	RockBehavior(Engine* e, GameObject* go);
	virtual void update();
};

class TankBehavior : public ObjectComponent {
public:
	TankBehavior(Engine* e, GameObject* go);
	virtual void update();
};



