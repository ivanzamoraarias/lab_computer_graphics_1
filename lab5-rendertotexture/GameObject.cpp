#include "GameObject.h"
#include "ObjectComponent.h"

GameObject::GameObject()
{
}

void GameObject::update()
{
	for (auto const& x : componentsMap) {
		if (x.first == componentType::RENDERABLE)
			continue;

		x.second->update();
	}

}

void GameObject::addComponent(ObjectComponent* a, componentType type)
{
	componentsMap[type] = a;
}

ObjectComponent* GameObject::getComponent(componentType type)
{
	if (componentsMap.empty()) {
		return nullptr;
	}
	return componentsMap[type];
}

void GameObject::updateComponent(std::pair< componentType, ObjectComponent*>& pair)
{
	pair.second->update();
}

void ObjectComponent::create(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}
