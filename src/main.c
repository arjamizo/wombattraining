/*******************************************************
* This game is designed for aviation students from     *
* Politechnika Rzeszowska (Poland). Before they are    *
* qualified for being pilot, they have to take some    *
* tests. One of the tests is Wombat (present website:  *
* http://www.aero.ca). I heard that results are        *
* suitable to player abilities, because nobody can     *
* train anything similar. Those, who say so, don't     *
* know about my program.                               *
*                                                      *
* This is my first program with SDL, openGL, GLU and   *
* freetype libraries. I had few days to prepare it     *
* and that's way, it's a little bit "junky". Hopefully,*
* it doesn't have too much bugs.                       *
* I didn't try to understand every part of the program.*
* Some of them are just copied from Internet.          *
* Fortunately, it works.                               *
*                                                      *
* Licence: GNU GPLv3                                   *
* Copyright bearh 2009                                 *
*******************************************************/

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "display.h"
#include "vars.h"
#include "wombat.h"
#include "events.h"
#include "timer.h"
#include "logging.h"
#include "menu.h"
#include "joystick.h"
#include "points.h"
#include "bonus1fig.h"
#include "scenario.h"

int main(int argc, char *argv[])
{
	wombatGameStatus *status=NULL;
	wombatTimer *timer=NULL;
	SDL_Surface *screen=NULL;
	SDL_Event event;
	//SDL_Joystick *joystick1=NULL, *joystick2=NULL;
	//SDL_Joystick **joysticks;
	ft_fontdata *fontdata=NULL;
	int timeNow, timeLast, timeDelta;
	
	wombatlog("Allocating memory for game data", 2);
	status = (wombatGameStatus *)malloc(sizeof(wombatGameStatus));  //create some memory for the game
	if (status == NULL)
	{
		wombatlog("Not enough memory for game data", 10);
		return 1;
	}
	wombatlog("Allocating memory for timers", 2);
	timer = (wombatTimer *)malloc(sizeof(wombatTimer));
	if (timer == NULL)
	{
		wombatlog("Not enough memory for timers", 10);
		free(status);
		return 1;
	}
	wombatlog("Allocating memory for fonts", 2);
	fontdata = (ft_fontdata *)malloc(sizeof(fontdata));
	if (fontdata == NULL)
	{
		wombatlog("Not enough memory for fonts", 10);
		free(timer);
		free(status);
		return 1;
	}
	wombatlog("Allocating memory for font textures", 2);
	fontdata->textures = (GLuint *)malloc(sizeof(GLuint)*128);
	if (fontdata->textures == NULL)
	{
		wombatlog("Not enough memory for font textures", 10);
		free(fontdata);
		free(timer);
		free(status);
		return 1;
	}
	
	SCENARIO_PATH[0] = '\0';
	status->scenario_name[0] = '\0';
	if (argc > 1)
	{
		wombatlog("Preparing for scenario loading", 2);
		if (scenario_check_name(argv[1]) == 0)
		{
			strncpy(status->scenario_name, argv[1], WOMBAT_FILENAME_SIZE);
			scenario_load_event(0);
		}
		else {
			status->scenario_name[0] = '\0';
		}
	}

	wombatlog("Setting default configuration", 2);
	setDefaultConfig();
printf("defaulted");
	loadConfig();
printf("loaded");
	/*wombatlog("Save default conf", 1);
	saveConfig();*/
	
	wombatlog("Initializing display", 2);
	if (initDisplay(screen) != 0)		//initialize display
	{
		wombatlog("Display init failed", 10);
		return 1;
	}

	
	
	wombatlog("Setting default values for game vars", 2);			
	wombatGameStatusReset(status);	

	/*SDL_JoystickEventState(SDL_ENABLE);		//initialize joysticks
	joystick1 = SDL_JoystickOpen(0);
	joystick2 = SDL_JoystickOpen(1);*/
	
	wombatlog("Preparing memory for bonus1 3d shapes", 2);
	Bonus1ReadFigures(status->scenario_name);
	wombatlog("Preparing font textures", 2);
	int er;
	if (er=ft_init(fontdata))
	{
		wombatlog("Font loading failed", 10);
		printf("%d\n",er);
		return 1;
	}

	//copyConfig2Vars();

	wombatTimersStart(timer);

	wombatlog("Initializing joysticks", 2);
	joystick_init();
	joystick_load();
	
//	status->mode=-getConfigInt("va_start");
//	VeloAcceChange(status, timer, getConfigInt("va_start"));

	wombatlog("Entering menu", 2);
	menu_main(status, timer, fontdata, &timeLast);
	
	copyConfig2Vars();

	wombatlog("Setting main task AI timer", 2);	
	timer->AI.timer = SDL_AddTimer(getConfigInt("ai_timerinterval"), AITimerFunction, NULL);	// before game begins set main AI timer
	timer->AI.run = 1;
	wombatlog("Setting changing velocity/acceleration mode timer", 2);
	timer->VeloAcce.timer = SDL_AddTimer(getConfigInt("va_timerinterval"), VeloAcceFunction, NULL);
	timer->VeloAcce.run = 1;

	if (getConfigInt("va_start"))
	{
		VeloAcceChange(status, timer);
	}

	wombatlog("Initializing random seed using current time", 2);
	srand((unsigned int)time(0));		//initialize random nr generator
	
	timeLast = SDL_GetTicks();		// prepare variables for time counting
	wombatlog("Enter main program loop", 2);
	while(status->timeleft != 0)		//main program loop
	{
		while(SDL_PollEvent(&event))
		{
			event_handler(&event, status, timer, fontdata, &timeLast);
		}
		
		timeNow = SDL_GetTicks();
		timeDelta = timeNow-timeLast;
		timeLast = timeNow;
		
		mainTaskUpdatePos(status, timeDelta);
		pointsCount(status, timeDelta);
		
		drawGLScene(status, fontdata);

		while(status->timeleft>0 && scenario_event_check((int)status->timeleft, status->scenario_event_nr, status, timer) > 0)
		{
			status->scenario_event_nr++;
		}

		if (status->timeleft > 0)
		{
			status->timeleft -= (float)timeDelta/1000;
			if (status->timeleft < 0)
			{
				status->timeleft = 0;
			}
		}
		pointsLog (status);
	}
	wombatlog("Ended main program loop", 2);
	
	wombatlog("Freeing 3dshapes memory", 2);
	Bonus1RemoveFigures(status);
	wombatlog("Ending SDL session", 2);
	SDL_Quit();	
	
	wombatlog("Free game data memory", 2);
	free(status);
	wombatlog("Free timer memory", 2);
	free(timer);
	wombatlog("Free font textures memory", 2);
	free(fontdata->textures);
	wombatlog("Free font memory", 2);
	free(fontdata);
	
	return 0;
}
