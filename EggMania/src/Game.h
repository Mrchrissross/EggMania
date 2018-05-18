#pragma once
#ifndef Game_h 
#define Game_h

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

#include "ScreenText.h"

class ColliderComponent;

class Game
{
private:						// Accessible only to other members of the same class

	float playerRunIncrease = 0.30f;	// How much the player speed will increase when they pickup an egg.

	int selection = 0;			// Used for menu selection.
	int tempSelection = 0;

	Mix_Music *BackGroundMusic1 = NULL;	//The music & Sound that will be played
	Mix_Chunk *SoundEffectButton = NULL;
	Mix_Chunk *SoundEffectPickUp = NULL;
	Mix_Chunk *SoundEffectLose = NULL;

	unsigned int currentTime = 0;	// Used to calculate time.
	int seconds = 0;				// arranging time in seconds and minutes.
	int minutes = 0;
	int cnt = 0;
	bool hasDied = false;			// bool to tell when the player has died.
	SDL_Window *window;			// Another pointer(*) called window of type SDL_Window

	unsigned int spawnTime = 5;		// Pickup spawn timer
	int pickupType = 0;				// Pickup egg type
	int pickupDeploy = 1;
	float spawnEgg;					// Randomiser

	unsigned int mineSpawnTime = 3;		// Mine spawn timer
	int mineType = 0;					// Type of mine whihc will spawn
	int mineDeploy = 1;					// mine deployment number

	unsigned int spawnCross = 2.9;		// how long after the player has died to spawn cross
	int positionXRandom;				// Position x on the map.
	int positionYRandom;				// Position y on the map.

	SDL_Color textColor = { 255, 255, 255, 0 };		// This is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Color textColorRed = { 255, 0, 0, 0 };		// This is used to give the text a red color
	char const *myScore;		// The score, converted to text..
	char const *myEnergy;		// The energy,
	char const *myTime;			// The Time,
	int score = 0;				// Set the score to an initial 0 when game starts.
	int tempScore = -1;			// Tempscore will be set to -1 as the game starts and then set to 0 which allows the score to appear on the screen.
	float energy;				// Used to calculate player energy (speed) 
	int newEnergy;				// Used as a conversion tool, to turn energy(float) to energy(int)
	std::string score1 = "score: ";
	std::string score2;
	std::string score3;
	std::string energy1 = "energy: ";				//These are all used for TTF purposes.
	std::string energy2;
	std::string energy3;
	std::string time1 = "time: ";
	std::string time2;
	std::string time3;
	std::string time4;
	std::string time5;

	// Declare all our text identifiers.
	ScreenText* scoreText;
	ScreenText* energyText;
	ScreenText* timeText;
	ScreenText* quitText;
	ScreenText* restartText;


public:							// Accessible from any place where the class is visible
	bool noRestart = false;
	bool isRunning;
	int gameState = 1;

	Game();						// Constructor method is called whenever an instance of the class is created and is named exactly as class name 
	~Game();					// Destructor method is called when a class is destroyed

	// methods in game
	
	void menuInit(const char* title, int width, int height, bool fullscreen);
	void init(const char* title, int width, int height, bool fullscreen);		// "Void" is used as the function will return no value to the caller
	
	// Menu/Game Loop -

	void handleEventsMenu();
	void handleEvents();
	void update();
	void menuUpdate();
	bool running() { return isRunning; }
	void render();
	void clean();		//	Memory management

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;		// Finding a random float for position randomisation and egg type.
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	static void AddTile(int id, int x, int y);					// Map Tile spawner.

	void Spawn(int x, int y);									// Spawn Egg
	void SpawnMine(int x, int y);								// Spawn Mine

	static SDL_Renderer* renderer;								// A pointer called renderer of type SDL_Renderer
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;			// Vector containing all colliders, was used for debuggin purposes during development.
};

#endif /*Game_h*/
