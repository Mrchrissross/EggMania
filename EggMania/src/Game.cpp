#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Timer.h"
#include <string>
#include <sstream>

Timer myTimer;		// Declare our timers,
Timer myTimer2;
Timer myTimer3;
Timer myTimer4;

Map* map;			// our map,
Manager manager;	// our manager,

SDL_Renderer* Game::renderer = nullptr;		// Our renderer,
SDL_Event Game::event;				// and our SDL_Event.

std::vector<ColliderComponent*> Game::colliders;

// Here we have all our entities, separated into sections declaring what they are.

#pragma region

auto& player(manager.addEntity());	

auto& youDied1(manager.addEntity());		// You died animation. Divided by 2 for memory management.
auto& youDied2(manager.addEntity());

#pragma region

auto& menuBackground(manager.addEntity());
auto& gameTitle(manager.addEntity());
auto& optionsText(manager.addEntity());

#pragma endregion MenuTexts

#pragma region
auto& topBorder(manager.addEntity());
auto& leftBorder(manager.addEntity());
auto& rightBorder(manager.addEntity());
auto& bottomBorder(manager.addEntity());

auto& disabledArea1(manager.addEntity());
auto& disabledArea2(manager.addEntity());
auto& disabledArea3(manager.addEntity());
auto& disabledArea4(manager.addEntity());
#pragma endregion Disabled Areas

#pragma region
auto& pickup1(manager.addEntity());
auto& pickup2(manager.addEntity());
auto& pickup3(manager.addEntity());
auto& pickup4(manager.addEntity());
auto& pickup5(manager.addEntity());
auto& pickup6(manager.addEntity());
auto& pickup7(manager.addEntity());
auto& pickup8(manager.addEntity());
#pragma endregion Pickups(8)

#pragma region
auto& mine1(manager.addEntity());
auto& mine2(manager.addEntity());
auto& mine3(manager.addEntity());
auto& mine4(manager.addEntity());
auto& mine5(manager.addEntity());
auto& mine6(manager.addEntity());
auto& mine7(manager.addEntity());
auto& mine8(manager.addEntity());
auto& mine9(manager.addEntity());
auto& mine10(manager.addEntity());
auto& mine11(manager.addEntity());
auto& mine12(manager.addEntity());
auto& mine13(manager.addEntity());
auto& mine14(manager.addEntity());
auto& mine15(manager.addEntity());
auto& mine16(manager.addEntity());
#pragma endregion Mines(16)

#pragma endregion Entities

// Here we add our groups enabling us to group certain entities together.

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupPickUps,
	groupTexts
};

// Here we set our groups to auto, so that when used in the render function each item in the specified group will be render in its correct layer.

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& pickups(manager.getGroup(groupPickUps));
auto& texts(manager.getGroup(groupTexts));

// Public methods

Game::Game()		// The class name is Game, and Game() is the constructor called from the public header file
{}

Game::~Game()		// The class name is Game, and ~Game() is the destructor called from the public header file
{}

void Game::menuInit(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;							// See Game::init for more info...
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)										// If SDL initilizes correctly
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD |	MIX_INIT_MP3 |	MIX_INIT_OGG);

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("Warning: Audio has not been found! \n");
	}
	else
	{
		SoundEffectButton = Mix_LoadWAV("../content/Button.wav");
		BackGroundMusic1 = Mix_LoadMUS("../content/ArcadeJD.mp3");
		Mix_VolumeChunk(SoundEffectButton, 128);			//The Volume of this sound (0-255)
		Mix_VolumeMusic(32);								//The volume for the music (0-255)
		std::cout << "Audio Initialised!..." << std::endl;
	}

	Mix_PlayMusic(BackGroundMusic1, -1);	//Play the music

	myTimer.start();

#pragma region

	menuBackground.addComponent<TransformComponent>(0.0f, 0.0f, 640, 800, 1.0f);
	menuBackground.addComponent<SpriteComponent>("../content/EggMania1.png");
	menuBackground.addGroup(groupPlayers);
	
	gameTitle.addComponent<TransformComponent>(360.0f, 20.0f, 211, 423, 1.0f);
	gameTitle.addComponent<SpriteComponent>("../content/EggMania2.png");
	gameTitle.addGroup(groupEnemies);

	optionsText.addComponent<TransformComponent>(50.0f, 0.0f, 650, 750, 1.0f);									// <--- This will be all options, play, settings and quit.
	optionsText.addComponent<SpriteComponent>("../content/Start.png");
	optionsText.addGroup(groupTexts);

	scoreText = new ScreenText(window, "../content/Roboto.ttf");
	energyText = new ScreenText(window, "../content/Roboto.ttf");
	timeText = new ScreenText(window, "../content/Roboto.ttf");

	quitText = new ScreenText(window, "../content/Roboto.ttf");
	restartText = new ScreenText(window, "../content/Roboto.ttf");

	std::cout << "Prefabs initialised!..." << std::endl;

#pragma endregion Prefabs
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;							// Here do not want any flags used, so we set this to zero.
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;		// If however in fullscreen, we will set our flags to have one, - SDL_WINDOW_FULLSCREEN
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)										// If SDL initilizes correctly
	{
		std::cout << "Subsystems Initialised!..." << std::endl;					// Tell the user,

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);		// create our window, under specified parametres
		renderer = SDL_CreateRenderer(window, -1, 0);																// create our renderer

		if (renderer)															// If our renderer is working..
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);				// set color to white.
		}

		isRunning = true;					// set our isRunning bool to true.
	}

	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("Warning: Audio has not been found! \n");
	}
	else
	{
		SoundEffectPickUp = Mix_LoadWAV("../content/Button.wav");
		SoundEffectLose = Mix_LoadWAV("../content/Lose.wav");
		BackGroundMusic1 = Mix_LoadMUS("../content/ArcadiaMix.mp3");
		Mix_VolumeChunk(SoundEffectPickUp, 128);			//The Volume of this sound (0-255)
		Mix_VolumeChunk(SoundEffectLose, 90);
		Mix_VolumeMusic(10);								//The volume for the music (0-255)
		std::cout << "Audio Initialised!..." << std::endl;
	}

	Mix_PlayMusic(BackGroundMusic1, -1);	//Play the music

	map = new Map();
	Map::LoadMap("../content/p25x20.map", 25, 20);
	std::cout << "Map Initialised!..." << std::endl;

	myTimer.start();
	myTimer2.start();
	myTimer3.start();
	std::cout << "Timer's Started!..." << std::endl;	

	// Entity Compnent System - Implementation

#pragma region

#pragma region

player.addComponent<TransformComponent>(500.0f, 400.0f, 32, 32, 2.0f);		// Controls the player's x, y coordinates, width and height and scale.
player.addComponent<SpriteComponent>("../content/player_anims.png", true);
player.addComponent<KeyboardController>();
player.addComponent<ColliderComponent>("player");
player.addGroup(groupPlayers);

#pragma endregion Players

std::cout << "Player Initialised!..." << std::endl;

#pragma region

scoreText = new ScreenText(window, "../content/Roboto.ttf");
energyText = new ScreenText(window, "../content/Roboto.ttf");
timeText = new ScreenText(window, "../content/Roboto.ttf");

quitText = new ScreenText(window, "../content/Roboto.ttf");
restartText = new ScreenText(window, "../content/Roboto.ttf");

#pragma endregion Texts

std::cout << "Texts Initialised!..." << std::endl;

#pragma region

leftBorder.addComponent<TransformComponent>(-20.0f, 0.0f, 650, 5, 1.0f);
leftBorder.addComponent<SpriteComponent>("../content/dirt.png");
leftBorder.addComponent<ColliderComponent>("LeftBorder");
leftBorder.addGroup(groupColliders);

rightBorder.addComponent<TransformComponent>(820.0f, 0.0f, 650, 5, 1.0f);
rightBorder.addComponent<SpriteComponent>("../content/dirt.png");
rightBorder.addComponent<ColliderComponent>("RightBorder");
rightBorder.addGroup(groupColliders);

topBorder.addComponent<TransformComponent>(0.0f, -20.0f, 5, 800, 1.0f);
topBorder.addComponent<SpriteComponent>("../content/dirt.png");
topBorder.addComponent<ColliderComponent>("TopBorder");
topBorder.addGroup(groupColliders);

bottomBorder.addComponent<TransformComponent>(0.0f, 640.0f, 5, 800, 1.0f);
bottomBorder.addComponent<SpriteComponent>("../content/dirt.png");
bottomBorder.addComponent<ColliderComponent>("BottomBorder");
bottomBorder.addGroup(groupColliders);

disabledArea1.addComponent<TransformComponent>(0.0f, 0.0f, 448, 44, 1.0f);
disabledArea1.addComponent<ColliderComponent>("Water");
disabledArea1.addGroup(groupColliders);

disabledArea2.addComponent<TransformComponent>(44.0f, 414.0f, 38, 342, 1.0f);
disabledArea2.addComponent<ColliderComponent>("Water");
disabledArea2.addGroup(groupColliders);

disabledArea3.addComponent<TransformComponent>(670.0f, 414.0f, 38, 65, 1.0f);
disabledArea3.addComponent<ColliderComponent>("Water");
disabledArea3.addGroup(groupColliders);

disabledArea4.addComponent<TransformComponent>(765.0f, 350.0f, 100, 55, 1.0f);
disabledArea4.addComponent<ColliderComponent>("Water");
disabledArea4.addGroup(groupColliders);

#pragma endregion Disabled Areas

std::cout << "Disabled Areas Initialised!..." << std::endl;

#pragma region
pickup1.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup1.addComponent<SpriteComponent>("../content/dirt.png");
pickup1.addComponent<ColliderComponent>("pickup1");
pickup1.addGroup(groupPickUps);

pickup2.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup2.addComponent<SpriteComponent>("../content/dirt.png");
pickup2.addComponent<ColliderComponent>("pickup2");
pickup2.addGroup(groupPickUps);

pickup3.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup3.addComponent<SpriteComponent>("../content/dirt.png");
pickup3.addComponent<ColliderComponent>("pickup3");
pickup3.addGroup(groupPickUps);

pickup4.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup4.addComponent<SpriteComponent>("../content/dirt.png");
pickup4.addComponent<ColliderComponent>("pickup4");
pickup4.addGroup(groupPickUps);

pickup5.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup5.addComponent<SpriteComponent>("../content/dirt.png");
pickup5.addComponent<ColliderComponent>("pickup1");
pickup5.addGroup(groupPickUps);

pickup6.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup6.addComponent<SpriteComponent>("../content/dirt.png");
pickup6.addComponent<ColliderComponent>("pickup2");
pickup6.addGroup(groupPickUps);

pickup7.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup7.addComponent<SpriteComponent>("../content/dirt.png");
pickup7.addComponent<ColliderComponent>("pickup3");
pickup7.addGroup(groupPickUps);

pickup8.addComponent<TransformComponent>(1000, 1000, 32, 32, 0.7f);
pickup8.addComponent<SpriteComponent>("../content/dirt.png");
pickup8.addComponent<ColliderComponent>("pickup4");
pickup8.addGroup(groupPickUps);
#pragma endregion Pickups

std::cout << "Pickups Initialised!..." << std::endl;

#pragma region
mine1.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine1.addComponent<SpriteComponent>("../content/dirt.png");
mine1.addComponent<ColliderComponent>("mine1");
mine1.addGroup(groupEnemies);

mine2.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine2.addComponent<SpriteComponent>("../content/dirt.png");
mine2.addComponent<ColliderComponent>("mine2");
mine2.addGroup(groupEnemies);

mine3.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine3.addComponent<SpriteComponent>("../content/dirt.png");
mine3.addComponent<ColliderComponent>("mine3");
mine3.addGroup(groupEnemies);

mine4.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine4.addComponent<SpriteComponent>("../content/dirt.png");
mine4.addComponent<ColliderComponent>("mine4");
mine4.addGroup(groupEnemies);

mine5.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine5.addComponent<SpriteComponent>("../content/dirt.png");
mine5.addComponent<ColliderComponent>("mine5");
mine5.addGroup(groupEnemies);

mine6.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine6.addComponent<SpriteComponent>("../content/dirt.png");
mine6.addComponent<ColliderComponent>("mine6");
mine6.addGroup(groupEnemies);

mine7.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine7.addComponent<SpriteComponent>("../content/dirt.png");
mine7.addComponent<ColliderComponent>("mine7");
mine7.addGroup(groupEnemies);

mine8.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine8.addComponent<SpriteComponent>("../content/dirt.png");
mine8.addComponent<ColliderComponent>("mine8");
mine8.addGroup(groupEnemies);

mine9.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine9.addComponent<SpriteComponent>("../content/dirt.png");
mine9.addComponent<ColliderComponent>("mine9");
mine9.addGroup(groupEnemies);

mine10.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine10.addComponent<SpriteComponent>("../content/dirt.png");
mine10.addComponent<ColliderComponent>("mine10");
mine10.addGroup(groupEnemies);

mine11.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine11.addComponent<SpriteComponent>("../content/dirt.png");
mine11.addComponent<ColliderComponent>("mine11");
mine11.addGroup(groupEnemies);

mine12.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine12.addComponent<SpriteComponent>("../content/dirt.png");
mine12.addComponent<ColliderComponent>("mine12");
mine12.addGroup(groupEnemies);

mine13.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine13.addComponent<SpriteComponent>("../content/dirt.png");
mine13.addComponent<ColliderComponent>("mine13");
mine13.addGroup(groupEnemies);

mine14.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine14.addComponent<SpriteComponent>("../content/dirt.png");
mine14.addComponent<ColliderComponent>("mine14");
mine14.addGroup(groupEnemies);

mine15.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine15.addComponent<SpriteComponent>("../content/dirt.png");
mine15.addComponent<ColliderComponent>("mine15");
mine15.addGroup(groupEnemies);

mine16.addComponent<TransformComponent>(1100, 1100, 32, 32, 0.7f);
mine16.addComponent<SpriteComponent>("../content/dirt.png");
mine16.addComponent<ColliderComponent>("mine16");
mine16.addGroup(groupEnemies);
#pragma endregion Mines

std::cout << "Mines Initialised!..." << std::endl;

youDied1.addComponent<TransformComponent>(1210.0f, 1120.0f, 398, 350, 1.0f);
youDied1.addComponent<SpriteComponent>("../content/youDied1.png");
youDied1.addGroup(groupTexts);

youDied2.addComponent<TransformComponent>(1210.0f, 1120.0f, 398, 350, 1.0f);
youDied2.addComponent<SpriteComponent>("../content/youDied2.png");
youDied2.addGroup(groupTexts);

std::cout << "All Prefabs initialised!..." << std::endl;

#pragma endregion Prefabs 

}

void Game::handleEventsMenu()
{
	SDL_PollEvent(&event);		// 

	if (event.type == SDL_KEYDOWN)		//Here we will handle the keyboard events
	{
		switch (event.key.keysym.sym)		//By pressing...
		{
		case SDLK_a:							//"A", you will move selection left
			selection = selection--;
			break;
		case SDLK_d:							//"D", you will move selection right
			selection = selection++;
			break;
		case SDLK_RETURN:						// When the user presses, enter...
			if (selection == 1)					// .. and if on selection one...
			{
				Mix_FreeMusic(BackGroundMusic1);
				Mix_PlayChannel(-1, SoundEffectButton, 0);
				SDL_Delay(500);
				gameState = 2;					// game state will change to two
				SDL_DestroyWindow(window);				// window will be destroyed
				SDL_DestroyRenderer(renderer);						// Renderer will be destroyed
				std::cout << "Menu Cleaned Successfully!..." << std::endl;	// Meaning the menu has been cleared
				std::cout << "Loading Game!..." << std::endl;		// Here we will now quit all our TTF, IMG and SDL,
				Mix_FreeChunk(SoundEffectButton);
				//IMG_Quit();							
				//SDL_Quit();
				isRunning = false;
			}
			if (selection == 2)					// .. and if on selection two...
			{
				clean();						// Go straight to the clean function,
				isRunning = false;				// Running becomes false,
				noRestart = true;				// and we do not want to restart, ultimately quitting the game.
			}
			break;
		case SDLK_v:		
			if (Mix_PlayingMusic() == 0)				//If there is no music playing
				Mix_PlayMusic(BackGroundMusic1, -1);	//Play the music
			else
				Mix_HaltMusic();
			break;
		default:
			break;
		}
	}

	switch (event.type)
	{
	case SDL_QUIT:				// This will handle the event in which you press the exit button making the game quit using SDL_Quit.
		clean();
		isRunning = false;		// The isRunning Boolean will turn false and the next time the game refreshes it will then exit.
		noRestart = true;
		break;

	default:
		break;
	}
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);		// 

	if (event.type == SDL_KEYDOWN)		//Here we will handle the keyboard events
	{
		switch (event.key.keysym.sym)		//By pressing...
		{
		case SDLK_v:
			if (Mix_PlayingMusic() == 0)				//If there is no music playing
				Mix_PlayMusic(BackGroundMusic1, -1);	//Play the music
			else
				Mix_HaltMusic();
			break;
		default:
			break;
		}
	}

	switch (event.type)
	{
	case SDL_QUIT:				// This will handle the event in which you press the exit button making the game quit using SDL_Quit.
		clean();
		isRunning = false;		// The isRunning Boolean will turn false and the next time the game refreshes it will then exit.
		noRestart = true;		// The noRestart Bool will turn true and this will allow the game to easily quit.
		break;
	case SDLK_v:
		if (Mix_PlayingMusic() == 0)				//If there is no music playing			
			Mix_PlayMusic(BackGroundMusic1, -1);	//Play the music
		else
			Mix_HaltMusic();
		break;
	default:
		break;
	}
}

void Game::menuUpdate()
{	
	// See update for more info.
	manager.refresh();
	manager.update();

	if (((myTimer.get_ticks() / 500) % 2) == 0)
	{
		gameTitle.getComponent<TransformComponent>().scale = 1.05f;
		gameTitle.getComponent<TransformComponent>().position.x = 320.0f;
		gameTitle.getComponent<TransformComponent>().position.y = 30.0f;
	}
	else
	{
		gameTitle.getComponent<TransformComponent>().scale = 1.0f;
		gameTitle.getComponent<TransformComponent>().position.x = 360.0f;
		gameTitle.getComponent<TransformComponent>().position.y = 20.0f;
	}

	if (selection != tempSelection)
	{
		switch (selection)
		{
		case 1:
			optionsText.getComponent<TransformComponent>().position.y = 0.0f;
			optionsText.getComponent<SpriteComponent>().setTex("../content/Start.png");
			break;
		case 2:
			optionsText.getComponent<TransformComponent>().position.y = 1.0f;
			optionsText.getComponent<SpriteComponent>().setTex("../content/Quit.png");
			break;
		default:
			break;
		}
		std::cout << selection << std::endl;
		tempSelection = selection;
	}

	if ((myTimer.get_ticks() / 100) == 1)
		selection = 1;

	if (selection == 0)
		selection = 2;
	else if (selection == 3)
		selection = 1;

	if (Mix_PlayingMusic() == 0)				//If there is no music playing
		timeText->setText("Press V to unmute music.", 500, 600, 30);
	else
		timeText->setText("Press V to mute music.", 530, 600, 30);
}

void Game::update()
{
	manager.refresh();													// This is our refresh function
	manager.update();													// This is where our manager will go through each enitity, checking their updates.

																		// Here is where we get the score and display it to the screen for the player to see.
	if (score != tempScore)
	{
		score2 = std::to_string(score);												// Conversion of score into a const char*
		score3 = score1 + score2;													//
		myScore = score3.c_str();													//
		
		scoreText->setText(myScore, 68, 0, 30);								// Setting the scoreText entity to display the complete score.

		energy = player.getComponent<TransformComponent>().speed - 2.55f;			// Energy will be set to how much the player speed currently is. - The higher the energy, the higher the player speed. 
		newEnergy = int(energy * 10);												// Conversion to int and also times ten, so each egg gives the player 1.5 energy.
		energy2 = std::to_string(newEnergy);										// Conversion of energy into a const char*
		energy3 = energy1 + energy2;												//
		myEnergy = energy3.c_str();													//	

		energyText->setText(myEnergy, 68, 27, 30);								// Setting the energyText entity to display the complete score.	

		tempScore = score;
	}

	// Start the timers													// Set game time and text
																				
	if ((myTimer.get_ticks() / 1000) != currentTime)
	{																				// If "my timer" is not equal to current time..
		std::cout << myTimer.get_ticks() / 1000 << std::endl;
		currentTime = myTimer.get_ticks() / 1000;									// Current time will be set to my timer.
		seconds++;																	// This used due to update() function, updating the ticks so many times per second.
																	 			
		if (seconds >= 60)														// This is used to count seconds and minutes
		{
			minutes++;
			seconds = 0;
		}

		time2 = std::to_string(minutes);											//
		time3 = std::to_string(seconds);											//
		time4 = time2 + ":" + time3;												// Conversion of time into a const char*
		time5 = time1 + time4;														//
		myTime = time5.c_str();														//
		
		timeText->setText(myTime, 650, 0, 30);
	}																			
		
#pragma region

	if ((myTimer2.get_ticks() / 1000) >= spawnTime)						// If timer2 is equal or above the spawnTime variable (5 seconds)...
	{
		if (((myTimer.get_ticks() / 1000) % spawnTime) == 0)			// If timer1 is also modules 5 equal to zero...
		{
			switch (pickupDeploy)	
			{
			case 1:
				pickup6.getComponent<TransformComponent>().position.x = 1000;	//pickup6 will have its x position turned to 1000, ...	
				pickup6.getComponent<TransformComponent>().position.y = 1000;	// ... and its y position turned to 1000, transporting it outside of the map. This will make the player think that they are destroyed but in reality they are not. This saves memory.
				//std::cout << "Pick Up 6 destroyed" << std::endl;																								//test: not destroyed but actually moved to outskirts of the map.
				pickupType = 1;				//the pickup type will be equal to one, making the next pick up to spawn is pickup1.

				pickupDeploy = 2;			// PickupDeploy will then be set to two making the next case happen.
				break;

			case 2:																// This will basically loop, transporting eggs outside the map and when its their turn again, back in. 
				pickup7.getComponent<TransformComponent>().position.x = 1000;
				pickup7.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 7 destroyed" << std::endl;
				pickupType = 2;

				pickupDeploy = 3;
				break;

			case 3:
				pickup8.getComponent<TransformComponent>().position.x = 1000;
				pickup8.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 8 destroyed" << std::endl;
				pickupType = 3;

				pickupDeploy = 4;
				break;

			case 4:
				pickup1.getComponent<TransformComponent>().position.x = 1000;
				pickup1.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 1 destroyed" << std::endl;
				pickupType = 4;

				pickupDeploy = 5;
				break;

			case 5:
				pickup2.getComponent<TransformComponent>().position.x = 1000;
				pickup2.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 2 destroyed" << std::endl; //test
				pickupType = 5;

				pickupDeploy = 6;
				break;

			case 6:
				pickup3.getComponent<TransformComponent>().position.x = 1000;
				pickup3.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 3 destroyed" << std::endl;
				pickupType = 6;

				pickupDeploy = 7;
				break;

			case 7:
				pickup4.getComponent<TransformComponent>().position.x = 1000;
				pickup4.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 4 destroyed" << std::endl;
				pickupType = 7;

				pickupDeploy = 8;
				break;

			case 8:
				pickup5.getComponent<TransformComponent>().position.x = 1000;
				pickup5.getComponent<TransformComponent>().position.y = 1000;
				//std::cout << "Pick Up 5 destroyed" << std::endl;
				pickupType = 8;

				pickupDeploy = 1;
				break;

			default:
				break;
			}
		}
	}

	while ((myTimer2.get_ticks() / 1000) >= spawnTime)	// Here I am implementing the spawn timer, spawnTime = 5 so an egg will spawn every 5 seconds in a random location.
	{
		positionXRandom = RandomFloat(80.0, 768.0);		// Finding a random position on the x axis
		positionYRandom = RandomFloat(32.0, 620.0);		// Finding a random position on the y axis

		if ((positionYRandom >= 370.0) && (positionYRandom <= 520.0))	// However between these two positions on the y axis is water, we dont want an egg to spawn where the player cannot get to.
		{
			positionXRandom = RandomFloat(420.0, 620.0);				// In this case the x axis will transform to only place an egg on the bridge area.
		}

		Game::Spawn(positionXRandom, positionYRandom);					// Spawn the egg

								//Reset the timer
		myTimer2.stop();								// myTimer2 (spawntimer) will be stopped	
		myTimer2.start();								// .. and reset.
		//std::cout << spawnTimer << std::endl;
		break;
	}

#pragma endregion Spawn Pickups Code

#pragma region
																		// See Spawn PickUp for more info...
	if ((myTimer3.get_ticks() / 1000) >= mineSpawnTime)
	{
		if (((myTimer.get_ticks() / 1000) % mineSpawnTime) == 0)
		{
			switch (mineDeploy)
			{
			case 1:
				mine10.getComponent<TransformComponent>().position.x = 1000;
				mine10.getComponent<TransformComponent>().position.y = 1000;
				mineType = 1;

				mineDeploy = 2;
				break;

			case 2:
				mine11.getComponent<TransformComponent>().position.x = 1000;
				mine11.getComponent<TransformComponent>().position.y = 1000;
				mineType = 2;

				mineDeploy = 3;
				break;

			case 3:
				mine12.getComponent<TransformComponent>().position.x = 1000;
				mine12.getComponent<TransformComponent>().position.y = 1000;
				mineType = 3;

				mineDeploy = 4;
				break;

			case 4:
				mine13.getComponent<TransformComponent>().position.x = 1000;
				mine13.getComponent<TransformComponent>().position.y = 1000;
				mineType = 4;

				mineDeploy = 5;
				break;

			case 5:
				mine14.getComponent<TransformComponent>().position.x = 1000;
				mine14.getComponent<TransformComponent>().position.y = 1000;
				mineType = 5;

				mineDeploy = 6;
				break;

			case 6:
				mine15.getComponent<TransformComponent>().position.x = 1000;
				mine15.getComponent<TransformComponent>().position.y = 1000;
				mineType = 6;

				mineDeploy = 7;
				break;

			case 7:
				mine16.getComponent<TransformComponent>().position.x = 1000;
				mine16.getComponent<TransformComponent>().position.y = 1000;
				mineType = 7;

				mineDeploy = 8;
				break;

			case 8:
				mine1.getComponent<TransformComponent>().position.x = 1000;
				mine1.getComponent<TransformComponent>().position.y = 1000;
				mineType = 8;

				mineDeploy = 9;
				break;

			case 9:
				mine2.getComponent<TransformComponent>().position.x = 1000;
				mine2.getComponent<TransformComponent>().position.y = 1000;
				mineType = 9;

				mineDeploy = 10;
				break;

			case 10:
				mine3.getComponent<TransformComponent>().position.x = 1000;
				mine3.getComponent<TransformComponent>().position.y = 1000;
				mineType = 10;

				mineDeploy = 11;
				break;

			case 11:
				mine4.getComponent<TransformComponent>().position.x = 1000;
				mine4.getComponent<TransformComponent>().position.y = 1000;
				mineType = 11;

				mineDeploy = 12;
				break;

			case 12:
				mine5.getComponent<TransformComponent>().position.x = 1000;
				mine5.getComponent<TransformComponent>().position.y = 1000;
				mineType = 12;

				mineDeploy = 13;
				break;

			case 13:
				mine6.getComponent<TransformComponent>().position.x = 1000;
				mine6.getComponent<TransformComponent>().position.y = 1000;
				mineType = 13;

				mineDeploy = 14;
				break;

			case 14:
				mine7.getComponent<TransformComponent>().position.x = 1000;
				mine7.getComponent<TransformComponent>().position.y = 1000;
				mineType = 14;

				mineDeploy = 15;
				break;

			case 15:
				mine8.getComponent<TransformComponent>().position.x = 1000;
				mine8.getComponent<TransformComponent>().position.y = 1000;
				mineType = 15;

				mineDeploy = 16;
				break;

			case 16:
				mine9.getComponent<TransformComponent>().position.x = 1000;
				mine9.getComponent<TransformComponent>().position.y = 1000;
				mineType = 16;

				mineDeploy = 1;
				break;

			default:
				break;
			}
		}
	}

	while ((myTimer3.get_ticks() / 1000) >= mineSpawnTime)		// Here I am implementing the spawn timer, spawnTime = 5, so a mine will spawn every 5 seconds in a random location.
	{
			positionXRandom = RandomFloat(80.0, 768.0);		// Finding a random position on the x axis
			positionYRandom = RandomFloat(32.0, 620.0);		// Finding a random position on the y axis

			if ((positionYRandom >= 370.0) && (positionYRandom <= 520.0))	// However between these two positions on the y axis is water, we dont want an egg to spawn where the player cannot get to.
			{
				positionXRandom = RandomFloat(420.0, 620.0);				// In this case the x axis will transform to only place a mine on the bridge area.
			}

			Game::SpawnMine(positionXRandom, positionYRandom);					// Spawn the mine		
									
								//Reset the timer
			myTimer3.stop();								// myTimer3 (Minespawntimer) will be stopped	
			myTimer3.start();								// .. and reset.

			break;
	}

#pragma endregion Spawn Mine Code

#pragma region

#pragma region

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,	// If the player collider(rect), intercepts ...
		leftBorder.getComponent<ColliderComponent>().collider))				// ... the left border collider(rect)...
	{
		if (hasDied == false)												// ... and if the player has not died...
		{
			player.getComponent<TransformComponent>().velocity * -1;		// the players velocity will be timed by -1 making the player bounce back from that object almost like to magnets colliding.
			std::cout << "Left Border Hit!" << std::endl;					// We will also display on the console that the player has hit the left border...
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,	// etc...
		rightBorder.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Right Border Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		topBorder.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Top Border Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		bottomBorder.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Bottom Border Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		disabledArea1.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Water Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		disabledArea2.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Water Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		disabledArea3.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Water Hit!" << std::endl;
		}
	}

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		disabledArea4.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Water Hit!" << std::endl;
		}
	}

#pragma endregion Disabled Areas

#pragma region

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,	// If the player collider(rect), intercepts ...
		pickup1.getComponent<ColliderComponent>().collider))				// ... the pickup1 collider(rect)...
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;													// The score wil go up by one.
		pickup1.getComponent<TransformComponent>().position.x = 1000;		// We will transport the egg outside the map.
		pickup1.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;	// We will increase the players speed/energy by .15f.

		std::cout << "Pickup Collected!" << std::endl;						// This will be displayed to the console as a log,
		std::cout << "Score: " << score << std::endl;						// as well as the score.
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup2.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup2.getComponent<TransformComponent>().position.x = 1000;
		pickup2.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup3.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup3.getComponent<TransformComponent>().position.x = 1000;
		pickup3.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup4.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup4.getComponent<TransformComponent>().position.x = 1000;
		pickup4.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup5.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup5.getComponent<TransformComponent>().position.x = 1000;
		pickup5.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup6.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup6.getComponent<TransformComponent>().position.x = 1000;
		pickup6.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup7.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup7.getComponent<TransformComponent>().position.x = 1000;
		pickup7.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		pickup8.getComponent<ColliderComponent>().collider))
	{
		Mix_PlayChannel(-1, SoundEffectPickUp, 0);

		score = score + 1;
		pickup8.getComponent<TransformComponent>().position.x = 1000;
		pickup8.getComponent<TransformComponent>().position.y = 1000;
		player.getComponent<TransformComponent>().speed = player.getComponent<TransformComponent>().speed + playerRunIncrease;

		std::cout << "Pickup Collected!" << std::endl;
		std::cout << "Score: " << score << std::endl;
	}

#pragma endregion Pickups

#pragma region
																			//	Mine 1 Collision with Player - See inside for info.
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine1.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)												// If the player has died yet..
		{
			mine1.getComponent<SpriteComponent>().setTex("");				// Mine's texture will be set to nothing (as it has blown up)

			player.getComponent<TransformComponent>().velocity * 0;					// In case the player is moving and prevent bugs, velocity will be set to zero.
			player.getComponent<KeyboardController>().movementEnabled = false;		// Then immediately after all player control and movement will be disabled.

			myTimer.stop();													// All timers will be halted to prevent any more spawns.
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();												// Timer 2, however will reset to allow for a player death animation to play
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;													// Once all is complete, Player has died will be set to "true".
		}										
	}
																			//	Mine 2 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine2.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine2.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 3 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine3.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine3.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 4 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine4.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine4.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 5 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine5.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine5.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 6 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine6.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine6.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 7 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine7.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine7.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 8 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine8.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine8.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 9 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine9.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine9.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 10 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine10.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine10.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 11 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine11.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine11.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 12 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine12.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine12.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 13 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine13.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine13.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 14 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine14.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine14.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 15 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine15.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine15.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}
																			//	Mine 16 Collision with Player
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		mine16.getComponent<ColliderComponent>().collider))
	{
		if (hasDied == false)
		{
			mine16.getComponent<SpriteComponent>().setTex("");

			player.getComponent<TransformComponent>().velocity * 0;
			player.getComponent<KeyboardController>().movementEnabled = false;

			myTimer.stop();
			myTimer2.stop();
			myTimer3.stop();
			myTimer2.start();
			myTimer4.start();

			std::cout << "Player has died!" << std::endl;
			std::cout << "Score: " << score << std::endl;
			hasDied = true;
		}
	}

#pragma endregion Mines

#pragma endregion Collision Check

#pragma region

	if (hasDied == true)									// Once the player has died...
	{
		if ((myTimer2.get_ticks() / 1000) == spawnCross)						// and if timer2 has reached the mark of spawnCross variable...
		{
			player.getComponent<SpriteComponent>().setTex("");									// The players texture will be set to nothing,
			myTimer2.stop();																					// myTimer2 will be stopped...				

			quitText->setText("Q = Quit", 25, 600, 30);
			restartText->setText("R = Restart", 650, 600, 30);
		}

		if ((myTimer4.get_ticks() / 100) < 0.5)
			Mix_PlayChannel(-1, SoundEffectLose, 0);

		if(((myTimer4.get_ticks() / 500) % 2) == 0)
		{
			youDied2.getComponent<TransformComponent>().position.x = 1210.0f;	// image two goes out of sight, image one comes in.
			youDied2.getComponent<TransformComponent>().position.y = 1120.0f;	// this was done for memory management.
			youDied1.getComponent<TransformComponent>().position.x = 210.0f;
			youDied1.getComponent<TransformComponent>().position.y = 120.0f;
		}
		else
		{
			youDied1.getComponent<TransformComponent>().position.x = 1210.0f;
			youDied1.getComponent<TransformComponent>().position.y = 1120.0f;
			youDied2.getComponent<TransformComponent>().position.x = 210.0f;
			youDied2.getComponent<TransformComponent>().position.y = 120.0f;
		}

		if (player.getComponent<KeyboardController>().isRunning == false)										// If the isRunning bool in the keyboard controller has been set to false...
		{
			if (player.getComponent<KeyboardController>().noRestart == true)					// ... and the noRestart bool has been set to false,
			{
				clean();														// The game will beginb cleaning and quitting processm
				noRestart = true;												// The Game class bool will then be set to true...
			}
			isRunning = false;													// ... and the isRunning bool will become false.
		}

		if ((player.getComponent<KeyboardController>().isRunning == false) && (player.getComponent<KeyboardController>().noRestart == false))
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			std::cout << "Game Cleaned Successfully!..." << std::endl;
			std::cout << "Loading Game!..." << std::endl;
		}
	}

#pragma endregion Player has died

}

void Game::render()
{
	SDL_RenderClear(renderer);							// This is where we would add the sources that we would like to render.
	
	for (auto& t : tiles)								//*********************************
	{
		t->draw();										//Here we will draw out our groups.
	}
	for (auto& p : players)								//*********************************
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	for (auto& pi : pickups)
	{
		pi->draw();
	}
	for (auto& txt : texts)
	{
		txt->draw();
	}

	scoreText->render();							//Constantly render the text -- we already have the texture
	energyText->render();
	timeText->render();
	quitText->render();
	restartText->render();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	Mix_FreeChunk(SoundEffectButton);
	Mix_FreeMusic(BackGroundMusic1);
	Mix_CloseAudio();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	std::cout << "Quitting!..." << std::endl;
	IMG_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned Successfully!..." << std::endl;
}

void Game::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);

	if(id == 2)
		tile.addComponent<ColliderComponent>("Water");
	else if (id == 5)
		tile.addComponent<ColliderComponent>("WaterFront");
	else if (id == 6)
		tile.addComponent<ColliderComponent>("WaterShadow");

	tile.addGroup(groupMap);
}

void Game::Spawn(int x, int y)
{
	spawnEgg = RandomFloat(5.0, 30.0); // Chooses a random egg to spawn.

	switch (pickupType)
	{
		case 1:
			pickup1.getComponent<TransformComponent>().position.x = x;
			pickup1.getComponent<TransformComponent>().position.y = y;
			
			if (spawnEgg <= 10.0)	
				pickup1.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup1.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup1.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup1.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup1.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 1 deployed!" << std::endl;
			break;

		case 2:
			pickup2.getComponent<TransformComponent>().position.x = x;
			pickup2.getComponent<TransformComponent>().position.y = y;
			
			if (spawnEgg <= 10.0)
				pickup2.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup2.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup2.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup2.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup2.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 2 deployed!" << std::endl;
			break;

		case 3:
			pickup3.getComponent<TransformComponent>().position.x = x;
			pickup3.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup3.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup3.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup3.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup3.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup3.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 3 deployed!" << std::endl;
			break;

		case 4:
			pickup4.getComponent<TransformComponent>().position.x = x;
			pickup4.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup4.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup4.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup4.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup4.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup4.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 4 deployed!" << std::endl;
			break;

		case 5:
			pickup5.getComponent<TransformComponent>().position.x = x;
			pickup5.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup5.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup5.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup5.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup5.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup5.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 5 deployed!" << std::endl;
			break;

		case 6:
			pickup6.getComponent<TransformComponent>().position.x = x;
			pickup6.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup6.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup6.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup6.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup6.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup6.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 6 deployed!" << std::endl;
			break;

		case 7:
			pickup7.getComponent<TransformComponent>().position.x = x;
			pickup7.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup7.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup7.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup7.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup7.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup7.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 7 deployed!" << std::endl;
			break;

		case 8:
			pickup8.getComponent<TransformComponent>().position.x = x;
			pickup8.getComponent<TransformComponent>().position.y = y;

			if (spawnEgg <= 10.0)
				pickup8.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg1.png");
			else if ((spawnEgg >= 10.0) && (spawnEgg <= 15.0))
				pickup8.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg2.png");
			else if ((spawnEgg >= 15.0) && (spawnEgg <= 20.0))
				pickup8.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg3.png");
			else if ((spawnEgg >= 20.0) && (spawnEgg <= 25.0))
				pickup8.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");
			else if ((spawnEgg >= 25.0) && (spawnEgg <= 30.0))
				pickup8.getComponent<SpriteComponent>().setTex("../content/PickUps/EasterEgg4.png");

			std::cout << "Pick Up 8 deployed!" << std::endl;
			break;

		default:
			break;
	}
}

void Game::SpawnMine(int x, int y)
{
	spawnEgg = RandomFloat(5.0, 25.0);

	switch (mineType)
	{
	case 1:
		mine1.getComponent<TransformComponent>().position.x = x;
		mine1.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine1.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine1.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine1.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 2:
		mine2.getComponent<TransformComponent>().position.x = x;
		mine2.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine2.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine2.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine2.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 3:
		mine3.getComponent<TransformComponent>().position.x = x;
		mine3.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine3.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine3.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine3.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 4:
		mine4.getComponent<TransformComponent>().position.x = x;
		mine4.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine4.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine4.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine4.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 5:
		mine5.getComponent<TransformComponent>().position.x = x;
		mine5.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine5.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine5.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine5.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 6:
		mine6.getComponent<TransformComponent>().position.x = x;
		mine6.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine6.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine6.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine6.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 7:
		mine7.getComponent<TransformComponent>().position.x = x;
		mine7.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine7.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine7.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine7.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 8:
		mine8.getComponent<TransformComponent>().position.x = x;
		mine8.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine8.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine8.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine8.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 9:
		mine9.getComponent<TransformComponent>().position.x = x;
		mine9.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine9.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine9.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine9.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 10:
		mine10.getComponent<TransformComponent>().position.x = x;
		mine10.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine10.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine10.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine10.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 11:
		mine11.getComponent<TransformComponent>().position.x = x;
		mine11.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine11.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine11.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine11.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 12:
		mine12.getComponent<TransformComponent>().position.x = x;
		mine12.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine12.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine12.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine12.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 13:
		mine13.getComponent<TransformComponent>().position.x = x;
		mine13.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine13.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine13.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine13.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 14:
		mine14.getComponent<TransformComponent>().position.x = x;
		mine14.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine14.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine14.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine14.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 15:
		mine15.getComponent<TransformComponent>().position.x = x;
		mine15.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine15.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine15.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine15.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;

	case 16:
		mine16.getComponent<TransformComponent>().position.x = x;
		mine16.getComponent<TransformComponent>().position.y = y;

		if (spawnEgg <= 12.0)
			mine16.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg1.png");
		else if ((spawnEgg >= 12.0) && (spawnEgg <= 19.0))
			mine16.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg2.png");
		else if (spawnEgg >= 19.0)
			mine16.getComponent<SpriteComponent>().setTex("../content/PickUps/BadEgg3.png");

		//std::cout << "Mine deployed!" << std::endl;
		break;
	default:
		break;
	}
}
