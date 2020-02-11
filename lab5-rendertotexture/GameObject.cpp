#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::update()
{
	for (auto const& x : componentsMap) {
		x.second->update(*this);
	}

}

void GameObject::addComponent(ObjectComponent* a, componentType type)
{
	componentsMap[type] = a;
}

ObjectComponent* GameObject::getComponent(componentType type)
{
	return componentsMap[type];
}

void GameObject::updateComponent(std::pair< componentType, ObjectComponent*>& pair)
{
	pair.second->update(*this);
}
