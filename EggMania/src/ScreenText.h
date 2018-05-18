/**
* @author Benjamin Williams <bwilliams@lincoln.ac.uk>
*
* @brief  A file which uses stb_truetype (stbtt) to render text to the screen.
* @notes  Consult the lecture slides for the usage of this class.
*/

//Include guard
#pragma once

//Import SDL


//Import standard libraries
#include <string>
#include <cstdarg>

#include "stb_truetype.h"
#include "SDL.h"



class ScreenText
{
public:

	/// <summary>
	/// Creates a new instance of a ScreenText object. Attaches it to the window, and loads in a font from a given path.
	/// </summary>
	/// <param name="window">The window which the text will be drawn to.</param>
	/// <param name="fontPath">The path to load the font from (for this text).</param>
	ScreenText(SDL_Window* window, const std::string& fontPath);

	/// <summary>
	/// Called when this instance has been destroyed.
	/// </summary>
	~ScreenText(void);

	/// <summary>
	/// Renders the text to screen.
	/// </summary>
	void render(void);

	/// <summary>
	/// Moves the text to another place on screen.
	/// </summary>
	/// <param name="x">The x position.</param>
	/// <param name="y">The y position.</param>
	void move(int x, int y);

	//Set colour
	void setColor(unsigned int color);
	void setColor(unsigned char r, unsigned char g, unsigned char b);
	void setColor(const SDL_Color color);

	/// <summary>
	/// Sets the text to render before render() is called.
	/// </summary>
	/// <param name="path">The text.</param>
	void setText(const std::string& text);

	/// <summary>
	/// Sets the text, along with the x, y position, (optional) font size and (optional) colour.
	/// </summary>
	/// <param name="text">The text.</param>
	/// <param name="posX">The x position.</param>
	/// <param name="posY">The y position.</param>
	/// <param name="fontSize">The font size.</param>
	/// <param name="r">The red component.</param>
	/// <param name="g">The green component.</param>
	/// <param name="b">The blue component.</param>
	void setText(const std::string& text, const int posX, const int posY, const int fontSize = 48, const int r = -1, const int g = -1, const int b = -1);

	/// <summary>
	/// Updates and remakes the surface after any state changes.
	/// </summary>
	void updateSurface(void);

private:

	//Helper functions
	bool loadFontData(const std::string& fontPath);
	void createTextSurface(void);

	//Internals/passed
	SDL_Renderer* renderer;
	SDL_Window* window;

	//Dimensions, positions & scale
	int width = 0, height = 0;
	int x = 0, y = 0;
	int fontScale = 48;

	//Current colour
	SDL_Color color;

	//Current text
	std::string text;

	//Buffers
	unsigned char* fontDataBuffer;
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;
};