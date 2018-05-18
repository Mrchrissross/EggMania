//
// EggMania!!!
//
// Created by Christopher Ross on 22/10/2017.
// Copyright © 2017 Christopher Ross. All rights reserved.
//
//

#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"
#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{
	const int FPS = 60;						// Framerate variables.
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	bool restart = true;					// Looping Variables
	bool programmeRunning = true;

	int gamestate = 1;						// Gamestate used to tell the programme what state the game is in - Menu or Game.

	while (programmeRunning)				// Programme Loop - to tell if the programme is running.
	{
		if (gamestate == 1)					// Menu State
		{
			game = new Game();									// New Game.
			game->menuInit("EggMania!", 800, 640, false);		// initiate the menu

			while (game->running())								// Game is now running.
			{
				frameStart = SDL_GetTicks();					// FrameStart is the amount SDL ticks.
				game->handleEventsMenu();						// Event menu has been intiated to watch for events. keyboard or 'x' button.
				game->menuUpdate();								// the update function where the main functions happen.
				game->render();									// Render to screen.

				gamestate = game->gameState;					// Always keep an eye out for the in game - gameState, changing, game state in main will always be the same. 

				if (game->noRestart == true)					// If the game has been set to "NOT" restart, 
				{
					programmeRunning = false;					// .. programmerunning will be set to false as we do not wish to restart and we wish to exit the loop/game.
				}

				frameTime = SDL_GetTicks() - frameStart;		// Frametime is SDL Ticks minus framestart

				if (frameDelay > frameTime)						// if the frameDelay is higher than the frametime.
				{
					SDL_Delay(frameDelay - frameTime);			// delay the sdl for a period of the amount of framedelay minus frametime.
				}
			}
		}

		if (gamestate == 2)
		{
			while (restart)
			{
				game = new Game();
				game->init("EggMania!", 800, 640, false);

				while (game->running())
				{
					frameStart = SDL_GetTicks();

					game->handleEvents();
					game->update();
					game->render();

					if (game->noRestart == true)
					{
						restart = false;
					}

					frameTime = SDL_GetTicks() - frameStart;

					if (frameDelay > frameTime)
					{
						SDL_Delay(frameDelay - frameTime);
					}
				}
			}
		}
		programmeRunning = false;							// once at the end of the programmerunning loop and all other loops have closed, programmerunning becomes false.
	}
	return 0;
}