#pragma once
#include "Transformable.h"
#include <iostream>

void Transformable::update()
{
	this->translateVector = vec3(1, 1, 1);
	this->scaleVector = vec3(1,1,1);
	this->rotateAngle = 0;
	this->rotateVector = vec3(1,1,1);
}

void Transformable::update(GameEntity& object) {
	std::cout << "Upddate Game Entity" << std::endl;
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
