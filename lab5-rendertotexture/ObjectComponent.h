#pragma once
#include "GameEntity.h"

enum componentType {
	TRANSFORMABLE,
	RENDERABLE ,
	AI ,
	RIGID_BODY
};

class ObjectComponent
{

public:
    //virtual void update()=0;
	virtual void update(GameEntity& g)=0;

};
