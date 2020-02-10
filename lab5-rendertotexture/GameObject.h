#pragma once
#include "ObjectComponent.h"
#include <vector>

using std::vector;

class GameObject
{
private:
	vector<ObjectComponent*> components;
public:
	GameObject();
	void update();
	void addComponent(ObjectComponent* a);

};

