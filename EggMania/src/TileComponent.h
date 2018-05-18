#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	SDL_Rect tileRect;
	int tileID;
	char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id)
	{
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		switch (tileID)
		{
		case 0:
			path = "../content/MapTiles/Tile_17.png";
			break;
		case 1:
			path = "../content/MapTiles/Tile_10.png";
			break;
		case 2:
			path = "../content/MapTiles/Tile_74.png";
			break;
		case 3:
			path = "../content/MapTiles/Tile_47.png";
			break;
		case 4:
			path = "../content/MapTiles/Tile_24.png";
			break;
		case 5:
			path = "../content/MapTiles/Tile_50.png";
			break;
		case 6:
			path = "../content/MapTiles/Tile_60.png";
			break;
		case 7:
			path = "../content/MapTiles/Tile_66.png";
			break;
		case 8:
			path = "../content/MapTiles/Tile_80.png";
			break;
		default:
			break;
		}
	}

	void init() override
	{
		entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1.0f);
		transform = &entity->getComponent<TransformComponent>();
		
		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->addComponent<SpriteComponent>();
	}
};