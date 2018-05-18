#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent *transform;					// create a reference to the transform comp
	SDL_Texture *texture;							// create textures
	SDL_Rect srcRect, destRect;						// create two rect

	bool animated = false;							// used to tell whether the sprite is animated or not.
	int frames = 0;									// amount of frames is set to 0
	int speed = 100;								// the speed of which the animations will go past is at 100ms.

public:

	int animIndex = 0;								// animIndex is set to 0

	std::map<const char*, Animation> animations;	// Our animations

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;	// SDL flip to flip our sprite in the event of going left

	SpriteComponent() = default;					
	SpriteComponent(const char* path)				// Our default sprite constructor, in which we can provide the local path to our sprite. ("../content/myPlayer.png)
	{
		setTex(path);															// call the set tex function to begin the process of turning our image into a texture.
	}

	SpriteComponent(const char* path, bool isAnimated)							// Our animated constructor, used for animated characters eg. spritecharts.
	{
		animated = isAnimated;							// Animated bool, turns true (or whatever I have picked) as this the animated constructor 

		Animation idle = Animation(0, 3, 100);			// 0 - Here we will oganise our 32x32 images into rows,
		Animation walk = Animation(1, 8, 100);			// 1 - On row 1, we have 8 images that i want to run at a speed of 100ms.
		Animation walkDown = Animation(2, 8, 100);		// 2 - On row 2, ....
		Animation walkUp = Animation(3, 8, 100);		// etc..
		Animation death1 = Animation(4, 30, 100);		// 4 - On row 4, we have a total of 30 images that will run at a speed of 100ms.

		animations.emplace("Idle", idle);				// Here we emplace the idle animation with the string "Idle", when ever this is called, idle animation will run.
		animations.emplace("Walk", walk);
		animations.emplace("WalkDown", walkDown);
		animations.emplace("WalkUp", walkUp);
		animations.emplace("Death1", death1);

		Play("Idle");									// Initiate the idle animation on game startup.

		setTex(path);									// Each image will go through the setTex function.
	}

	~SpriteComponent()
	{
		//	//if((texture != nullptr) || (texture != NULL) || texture != 0x0000000000000000)	// -----------Using these SDL_DestroyTexture's seems to crash the game after pressing quit to exit the game.
		//	SDL_DestroyTexture(texture);														// Not using these seems to make the game run much more smoothly, no crashes, and no memory leakage.-----------
		//	//if ((textTexture != nullptr) || (textTexture != NULL))
		//	SDL_DestroyTexture(textTexture);
	}

	void setTex(const char* path)						// Our main texture creating function, called whenever we would like to create a texture from an image(.png)
	{
		texture = TextureManager::LoadTexture(path);	// Pass our image path through to the texture manager.
	}

	void init() override											// Here we override the init function from the Game.
	{
		transform = &entity->getComponent<TransformComponent>();	// From here on we will set our rects (passed through the transfrom component) in which we want to have pasted the texture onto.

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override											// Here we render our sprites/rects/textures.
	{	
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)									// Our play function turns a sprite sheet into the animation we want.
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};

// This was used for TTF implementation, this does not work.

// bool isFont = false;							// used to tell whether this is a font/text object.
//SpriteComponent(const char* font_path, int font_size, const char* message_text, const SDL_Color &color)		// Our font constructor, allowing us to transform a text font image into a texture.
//{
//	isFont = true;																									
//	addText(font_path, font_size, message_text, color);
//}
//void addText(const char* font_path, int font_size, const char* message_text, const SDL_Color &color)	// Same operation as the setTex function, except here we will pass in path to our font, font size, the message we want to emit, and the color. 
//{
//	isFont = true;
//	textTexture = TextureManager::LoadFont(font_path, font_size, message_text, color);
//}
// Inside draw()
//	if(isFont)
//		TextureManager::Draw(textTexture, srcRect, destRect, spriteFlip);
//	else