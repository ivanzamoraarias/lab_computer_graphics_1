#pragma once
#include "ObjectComponent.h"
#include "GameEntity.h"
#include <algorithm>
#include <vector>
#include <map>

using std::vector;
using std::map;

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

