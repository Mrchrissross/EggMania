#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface *tempSurface = IMG_Load(texture);									// Turn the image into a surface.
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);	// Use the surface to create a texture.
	SDL_FreeSurface(tempSurface);													// Once texture is create, we no longer need the surface so we can free it up.

	return tex;																		// return the texture to the sprite component.
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);			// drawing the texture to the renderer.
}


// TTF Implementation, this does not work.

//TTF_Font* font;

//SDL_Texture* TextureManager::LoadFont(const char* font_path, int font_size,			// This is essentially the same as the load texture.				
//	const char* message_text, const SDL_Color &color)
//{
//	font = TTF_OpenFont(font_path, font_size);												// Only difference that we are gather the font we need and getting the size,
//	SDL_Surface *textSurface = TTF_RenderText_Solid(font, message_text, color);				// using the message text and color to make the font create text and turn that text into an image.
//	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);	// Again turn the surface into a texture.
//	SDL_FreeSurface(textSurface);															// Free the surface.
//
//	return textTexture;																		// Return the texture.
//}
//
//void TextureManager::close()																// This is apart of the cleaning process, used to completely close global fonts. 
//{
//	TTF_CloseFont(font);																	// To only be used when *Quiting the game*.
//	font = NULL;
//}
