#pragma once

#include "Game.h"

class TextureManager
{
	public:
		static SDL_Texture* LoadTexture(const char* fileName);
		static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);

		//TTF Implementation, This does not work.
		/*static SDL_Texture* LoadFont(const char* font_path, int font_size,
			const char* message_text, const SDL_Color &color);
		static void close();*/
};