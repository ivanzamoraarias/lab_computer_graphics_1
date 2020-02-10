#pragma once
#include "Transformable.h"

void Transformable::update()
{
}

void Transformable::update(GameObject& object) {

}

void Transformable::setTransLate(vec3 translate)
{
	this->translateVector = translate;
}

void Transformable::setScale(vec3 scale)
{
	this->scaleVector = scale;
}

void Transformable::setRotate(vec3 rotate, float angle)
{
	this->rotateVector = rotate;
	this->rotateAngle = angle;
}

mat4 Transformable::getTransformationMatrix()
{
	mat4 resultingMatrix = 
		translate(translateVector)*
		rotate(rotateAngle, rotateVector)*
		scale(scaleVector);

	return resultingMatrix;
}

Transformable::Transformable(/* args */)
{
	this->rotateAngle = 0.0f;
}

Transformable::~Transformable()
{
}
