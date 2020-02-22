#ifndef GAME_OBJECT_h

#define GAME_OBJECT_h



#pragma once


#include "GameEntity.h"
#include "ObjectComponent.h";
#include <algorithm>
#include <vector>
#include <map>

using std::vector;
using std::map;

class Engine;

enum componentType {
	TRANSFORMABLE,
	RENDERABLE,
	AI,
	RIGID_BODY
};
class ObjectComponent;

class GameObject: public GameEntity 
{
private:
	map<componentType, ObjectComponent*> componentsMap;
	void updateComponent(std::pair<componentType, ObjectComponent*>& pair);


public:
	GameObject();
	void update();
	void addComponent(ObjectComponent* a, componentType type);
	ObjectComponent* getComponent(componentType type);

};


class ObjectComponent
{
protected:
	GameObject* gameObject;
	Engine* engine;

public:
	virtual void create(Engine* e, GameObject* go);
	virtual void update() = 0;

};

#endif // GAME_OBJECT_h