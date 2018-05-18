#include "Timer.h"

Timer::Timer()
{
	startTicks = 0;		//Initialize the variables
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	started = true;							//Start the timer	
	paused = false;							//Unpause the timer	
	startTicks = SDL_GetTicks();			//Get the current clock time
}

void Timer::stop()
{	
	started = false;						//Stop the timer
	paused = false;							//Unpause the timer
}

int Timer::get_ticks()
{
	if (started == true)								//If the timer is running
	{
		if (paused == true)								//If the timer is paused
		{
			return pausedTicks;							//Return the number of ticks when the timer was paused
		}
		else
		{
			return SDL_GetTicks() - startTicks;			//Return the current time minus the start time
		}
	}
	return 0;											//If the timer isn't running
}

void Timer::pause()
{
	if ((started == true) && (paused == false))			//If the timer is running and isn't already paused
	{
		paused = true;									//Pause the timer
		pausedTicks = SDL_GetTicks() - startTicks;		//Calculate the paused ticks
	}
}

void Timer::unpause()
{
	if (paused == true)									//If the timer is paused
	{
		paused = false;									//Unpause the timer
		startTicks = SDL_GetTicks() - pausedTicks;		//Reset the starting ticks
		pausedTicks = 0;								//Reset the paused ticks
	}	
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}