#pragma once

//*********************************************************************************************************
//
//	This file will be used to control keyboard events, allowing the player to perform actions in the game.
//
//*********************************************************************************************************

#include "Game.h"
#include "ECS.h"
#include "Components.h"
													
class KeyboardController : public Component
{
public:
	bool isRunning = true;
	bool noRestart = false;

	TransformComponent *transform;
	SpriteComponent *sprite;

	bool movementEnabled = true;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override										
	{
		while (movementEnabled)
		{
			if (Game::event.type == SDL_KEYDOWN)		//Here we will handle the keyboard events
			{
				switch (Game::event.key.keysym.sym)		//By pressing...
				{
				case SDLK_w:							//"W", you will move Forward
					transform->velocity.y = -1;
					sprite->Play("WalkUp");				//Play walking animation
					break;
				case SDLK_a:							//"A", you will move Left
					transform->velocity.x = -1;
					sprite->Play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;		//This is used to flip the sprite around to walk left.
					break;
				case SDLK_d:							//"D", you will move Right
					transform->velocity.x = 1;
					sprite->Play("Walk");
					break;
				case SDLK_s:							//"S", you will move Backward
					transform->velocity.y = 1;
					sprite->Play("WalkDown");
					break;
				default:
					break;
				}
			}

			if (Game::event.type == SDL_KEYUP)
			{
				switch (Game::event.key.keysym.sym)		//Once the key has been released, our velocity will return to the default of 0.
				{
				case SDLK_w:
					transform->velocity.y = 0;
					sprite->Play("Idle");
					break;
				case SDLK_a:
					transform->velocity.x = 0;
					sprite->Play("Idle");
					sprite->spriteFlip = SDL_FLIP_NONE;
					break;
				case SDLK_d:
					transform->velocity.x = 0;
					sprite->Play("Idle");
					break;
				case SDLK_s:
					transform->velocity.y = 0;
					sprite->Play("Idle");
					break;
				default:
					break;
				}
			}
			break;			
		}
		if (movementEnabled == false)
		{
			sprite->Play("Death1");

			if (Game::event.type == SDL_KEYDOWN)		
			{
				switch (Game::event.key.keysym.sym)		
				{
				case SDLK_q:
					isRunning = false;
					noRestart = true;
					break;
				case SDLK_r:				
					isRunning = false;
					break;
				default:
					break;
				}
			}
		}
	}
};