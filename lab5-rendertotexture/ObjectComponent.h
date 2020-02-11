#pragma once
#include "GameEntity.h"

class ObjectComponent
{

public:
    //virtual void update()=0;
	virtual void update(GameEntity& g)=0;

};
