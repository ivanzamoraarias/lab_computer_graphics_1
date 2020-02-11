#pragma once
#include "ObjectComponent.h"
#include "GameEntity.h"
#include <algorithm>
#include <vector>
#include <map>

using std::vector;
using std::map;

enum componentType {
	TRANSFORMABLE=0,
	RENDERABLE=1,
	AI=2,
	RIGID_BODY=3
};

class GameObject: public GameEntity 
{
private:
	vector<ObjectComponent*> components;
	map<int, ObjectComponent*> componentsMap;
	void updateComponent(std::pair<componentType, ObjectComponent*>& pair);


public:
	GameObject();
	void update();
	void addComponent(ObjectComponent* a, componentType type);
	ObjectComponent* getComponent(componentType type);

};

