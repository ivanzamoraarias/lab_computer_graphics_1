#pragma once
#include "ObjectComponent.h"
#include "GameObject.h">
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;
 

class Transformable: public ObjectComponent
{
private:
	vec3 translateVector;
	vec3 scaleVector;
	vec3 rotateVector;
	float rotateAngle;
public:
	void update();
    void update(GameObject& object);
	void setTransLate(vec3 translate);
	void setScale(vec3 scale);
	void setRotate(vec3 rotate, float angle);

	mat4 getTransformationMatrix();

	Transformable(/* args */);
    ~Transformable();
};

