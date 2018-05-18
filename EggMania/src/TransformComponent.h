#pragma once

#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component			// The difference between a struct and a class is that everything by default in a class is private,
{														// however everything in a struct by default is public.
public:

	Vector2D position;
	Vector2D velocity;

	int height = 32;	
	int width = 32;
	float scale = 1.0f;	//int

	float speed = 2.55f;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc)
	{
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.Zero();
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();							//Here we will set our default velocity.							
	}

	void update() override
	{
		position.x += velocity.x * speed;		//This gives our character movement,
		position.y += velocity.y * speed;		//so our velocity will move us forward from our position multiplied by our speed.
	}
};
