/**
* @author Benjamin Williams <bwilliams@lincoln.ac.uk>
*
* @brief  A file which uses stb_truetype (stbtt) to render text to the screen.
* @notes  Consult the lecture slides for the usage of this class.
*/

//Include header file for definitions & prototypes
#include "ScreenText.h"

//Define RGBA mask for later on, depending on if SDL is storing colours in little/big endian format
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

/// <summary>
/// Called when this instance has been initialised.
/// </summary>
ScreenText::ScreenText(SDL_Window* window, const std::string& path)
{
	//Set window and renderer
	this->window = window;
	this->renderer = SDL_GetRenderer(window);

	//Couldn't open font data
	if (!loadFontData(path))
	{
		//Log error message and return
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't open font file '%s' or read data.", path);
		return;
	}

	//Constructed.. set default colour
	this->setColor(255, 255, 255);
}


/// <summary>
/// Called when this instance has been destroyed.
/// </summary>
ScreenText::~ScreenText(void)
{
	//Delete font file data
	delete[] fontDataBuffer;

	//Destroy existing surfaces and textures
	SDL_FreeSurface(this->textSurface);

	//If the texture is not null.. destroy it
	if (this->textTexture != NULL)
		SDL_DestroyTexture(this->textTexture);
}


/// <summary>
/// Updates and remakes the surface after any state changes.
/// </summary>
void ScreenText::updateSurface(void)
{
	//Do a bit of cheating, remake surface with same string.. updating it :)
	this->setText(this->text);
}


/// <summary>
/// Creates an text surface.. destroys existing textures and surfaces too.
/// </summary>
void ScreenText::createTextSurface(void)
{
	//Destroy existing surfaces and textures
	SDL_FreeSurface(this->textSurface);

	//If the texture is not null.. destroy it
	if (this->textTexture != NULL)
		SDL_DestroyTexture(this->textTexture);

	//Window width and height
	int windowWidth, windowHeight;

	//Firstly, find the width/height of the window
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	//Set width/height
	this->width = windowWidth;
	this->height = windowHeight;

	//Create a surface that big (but only a bit depth of 8)
	this->textSurface = SDL_CreateRGBSurface(0, this->width, this->height, 8, 0, 0, 0, 0);

	//Create colours for palette
	SDL_Color colors[256];
	//--
	for (unsigned i = 0; i < 256; i++)
	{
		//Find percentage from (0 -> 256), scalar-fixed lerp
		float percent = (i / 256.0f);

		//Set the RGB components functionally
		colors[i] = this->color;

		//Change alpha value to this percent (fade out towards edges)
		colors[i].a = (int)(percent * 256.0f);
	}

	//Set palette of text surface
	SDL_SetPaletteColors(this->textSurface->format->palette, colors, 0, 256);
}


/// <summary>
/// Renders the text to screen.
/// </summary>
void ScreenText::render(void)
{
	//Source rect is the full width of the text
	SDL_Rect src = { 0, 0, this->width, this->height };

	//Dest rect is using internal state of this ScreenText instance
	SDL_Rect dst = { this->x, this->y, this->width, this->height };

	//Use rendercopy to copy using these parameters
	SDL_RenderCopy(renderer, this->textTexture, &src, &dst);
}


/// <summary>
/// Sets the colour of the text to rendered.
/// </summary>
/// <param name="color">The RGB_888 integer representing a colour.</param>
void ScreenText::setColor(unsigned int color)
{
	//Extract R G and B channels, mask with 0xff
	unsigned char
		r = (color >> 16) & 0xff,
		g = (color >> 8) & 0xff,
		b = (color >> 0) & 0xff;

	//Set the colour
	this->setColor(r, g, b);
}

/// <summary>
/// Sets the colour of the text to rendered.
/// </summary>
/// <param name="r">The r component [0, 255].</param>
/// <param name="g">The g component [0, 255].</param>
/// <param name="b">The b component [0, 255].</param>
void ScreenText::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	//Make a new colour
	SDL_Color color = { r, g, b };

	//Call other overloaded setColor function
	this->setColor(color);
}


/// <summary>
/// Sets the colour of the text to rendered.
/// </summary>
/// <param name="color">The colour.</param>
void ScreenText::setColor(SDL_Color color)
{
	//Set up colour
	this->color = color;

	//Update surface
	this->updateSurface();
}


/// <summary>
/// Moves the text to another place on screen.
/// </summary>
/// <param name="x">The x position.</param>
/// <param name="y">The y position.</param>
void ScreenText::move(int x, int y)
{
	//Set internal x and y
	this->x = x;
	this->y = y;
}

/// <summary>
/// Sets the text, along with the x, y position and font size.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="posX">The x position.</param>
/// <param name="posY">The y position.</param>
/// <param name="fontSize">The font size.</param>
/// <param name="r">The red component.</param>
/// <param name="g">The green component.</param>
/// <param name="b">The blue component.</param>
void ScreenText::setText(const std::string& text, const int posX, const int posY, const int fontSize, const int r, const int g, const int b)
{
	//Set x, y and font scale
	this->x = posX;
	this->y = posY;
	this->fontScale = fontSize;

	//If parameters have been changed
	if (r != -1 && g != -1 && b != -1)
		this->setColor(r, g, b);

	//Call set text normally
	setText(text);
}


/// <summary>
/// Sets the text to render before render() is called.
/// </summary>
/// <param name="path">The text.</param>
void ScreenText::setText(const std::string& text)
{
	//Set text!
	this->text = text;

	//Create text surface
	createTextSurface();

	//Create a new array (for bitmap character texture lookup)
	stbtt_bakedchar charData[96];

	//Find font offset for font data so we can read it
	int offset = stbtt_GetFontOffsetForIndex(this->fontDataBuffer, 0);

	//Build a bitmap texture for every character, for this font (a texture atlas)
	stbtt_BakeFontBitmap(this->fontDataBuffer, offset, this->fontScale,    //Font data to read, offset and scale
		(unsigned char*)textSurface->pixels,              //The pixels of the text surface (to copy into)
		this->width, this->height,                        //Dimensions of array to copy into
		32,                                               //Bit depth (32, RGBA_8888)
		96,                                               //96 characters in atlas
		charData);                                        //The atlas to save into

														  //Create an offset surface to blit to before turning into a texture
	SDL_Surface* offscreenSurface = SDL_CreateRGBSurface(0, this->width, this->height, 32, RMASK, GMASK, BMASK, AMASK);

	//Set color key for blitting (skip black pixels -- they're transparent)
	SDL_SetColorKey(textSurface, SDL_TRUE, 0);
	SDL_SetColorKey(offscreenSurface, SDL_TRUE, 0);

	//Current X and Y on baked quads
	float x = 0, y = 0;

	for (const char& c : text)
	{
		//Run through each character in the string
		//..

		//Find quad to save into (aligned, not rotated)
		stbtt_aligned_quad quad;

		//Get the quad for this character, save into 'quad'
		stbtt_GetBakedQuad(charData, this->width, this->height, (c - 32), &x, &y, &quad, true);

		//Calculate width of quad and height of quad -- just (r - l) and (t - b)
		int w = (quad.x1 - quad.x0);
		int h = (quad.y1 - quad.y0);

		//Calculate source rect for this quad
		SDL_Rect src = { quad.s0 * this->width, quad.t0 * this->height, w, h };

		//Calculate dest rect (in offscreen) for this quad
		SDL_Rect dest = { quad.x0, fontScale + quad.y0, w, h };

		//Blit to offscreen (temp surface)
		SDL_BlitSurface(textSurface, &src, offscreenSurface, &dest);
	}

	//Disable color key.. we may need black text!
	SDL_SetColorKey(textSurface, SDL_FALSE, 0);
	SDL_SetColorKey(offscreenSurface, SDL_FALSE, 0);

	//Set text texture (on GPU) to offscreen surface
	this->textTexture = SDL_CreateTextureFromSurface(renderer, offscreenSurface);

	//Free offscreen surface
	SDL_FreeSurface(offscreenSurface);
}


/// <summary>
/// Loads font data from a given path and saves it into this instance.
/// </summary>
/// <param name="path">The path.</param>
bool ScreenText::loadFontData(const std::string& path)
{
	//Log to tell the user we're starting
	SDL_Log("Font loading started! Path is '%s'", path.c_str());

	//Try to open up the file 
	FILE* fontFile = fopen(path.c_str(), "rb");

	//Couldn't be opened. Return false.
	if (!fontFile)
		return false;

	//File size to save into
	long size = 0;

	//Go to the end of the file. Whatever position the cursor is at is how long the file is.
	fseek(fontFile, 0, SEEK_END);
	size = ftell(fontFile);

	//Reset back to the start
	fseek(fontFile, 0, SEEK_SET);

	//Allocate however many bytes are needed to store this file's content in RAM
	this->fontDataBuffer = new unsigned char[size];

	//Read entire file into RAM and then close it
	fread(fontDataBuffer, size, 1, fontFile);
	fclose(fontFile);

	//Return true -- everything is good!
	return true;
}