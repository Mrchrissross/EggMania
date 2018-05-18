#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())			//If the entity does not have the transform component,  
		{
			entity->addComponent<TransformComponent>();				//We will automatically add it to prevent any errors.
		}
		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * static_cast<int>(transform->scale);			///<---
		collider.h = transform->height * static_cast<int>(transform->scale);			///<---
	}

};
