#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::update()
{
}

void GameObject::addComponent(ObjectComponent* a)
{
	components.push_back(a);
}
