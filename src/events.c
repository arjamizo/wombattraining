#include "events.h"

int event_handler(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer, ft_fontdata *fontdata, int *timeLast)
{
	char text[30];

	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
	{
		wombatlog("Event: ESC", 2);
		status->timeleft=0;
	}

	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_m)
	{
		wombatlog("Event: Menu", 2);
		menu_main(status, timer, fontdata, timeLast);
	}

	if (event->type == SDL_USEREVENT)						//timers
	{
		switch (event->user.code)
		{
			case 0:	// AI
				wombatlog("Event: timer: main AI", 1);
				mainTaskAI(&(status->x));
				mainTaskAI(&(status->y));
				mainTaskAI(&(status->z));
				break;

			case 1: // Autotrack
				wombatlog("Event: timer: autotrack", 1);
				mainTaskAuto(&(status->x), status->mode, SDL_GetTicks()-status->at_timelast);
				mainTaskAuto(&(status->y), status->mode, SDL_GetTicks()-status->at_timelast);
				mainTaskAuto(&(status->z), status->mode, SDL_GetTicks()-status->at_timelast);
				status->at_timelast = SDL_GetTicks();
				break;
			case 2:	// Autotrack Failure
				wombatlog("Event: timer: failure", 1);
				AutoFail(status, timer);
				break;
			case 3: // Autotrack Repair
				wombatlog("Event: timer: repair", 1);
				AutoRepair(status, timer);
				break;
			case 4: // Velocity Acceleration Switch
				wombatlog("Event: timer: velo accel switch", 1);
				VeloAcce(status, timer);
				break;
			case 5: // Acceleration update
				wombatlog("Event: timer: acceleration update", 1);
				AccelUpdate(status);
				break;
			case 6:	// Bonus timeleft
				wombatlog("Event: timer: Bonus timeleft", 1);
				BonusRemoveTime(status, timer);
				break;
			case 7: // Bonus3 show time
				wombatlog("Event: timer: Bonus3 1st timer", 1);
				Bonus31(status, timer);
				break;
			case 8: // Bonus3 hide time
				wombatlog("Event: timer: Bonus3 2nd timer", 1);
				Bonus32(status, timer);
				break;
		}
		return 0;				// Only here use timers
	}

	if (status->ifbonus)
	{
		switch (status->whichbonus)
		{
			case 0:
				event_bonus0(event, status, timer);
				break;
			case 1:
				event_bonus1(event, status, timer);
				break;
			case 2:
				event_bonus2(event, status, timer);
				break;
			case 3:
				event_bonus3(event, status, timer);
				break;
		}
		return 0;
	}
	else 
	{
		event_main(event, status, timer);
		return 0;
	}
}

int event_bonus0(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_1:	// select bonus1
					wombatlog("Event: Choose bonus 1", 1);
					status->whichbonus=1;
					pointsBonusValueUpdate(status, 1);
					timer->Bonus.timer = SDL_AddTimer(getConfigInt("bonus_removeinterval"), BonusFunction, NULL);
					timer->Bonus.run = 1;
					status->bonustimeleft = getConfigInt("bonus_time");
					Bonus1start(status);
					break;
				case SDLK_q:	// select bonus2
					wombatlog("Event: Choose bonus 2", 1);
					status->whichbonus=2;
					pointsBonusValueUpdate(status, 2);
					status->pbperformance = getConfigInt("points_performance_max")/2;
					timer->Bonus.timer = SDL_AddTimer(getConfigInt("bonus_removeinterval"), BonusFunction, NULL);
					timer->Bonus.run = 1;
					status->bonustimeleft = getConfigInt("bonus_time");
					Bonus2start(status);
					break;
				case SDLK_a:	// select bonus3
					wombatlog("Event: Choose bonus 3", 1);
					status->whichbonus=3;			
					pointsBonusValueUpdate(status, 3);	
					status->pbperformance = getConfigInt("points_performance_max")/2;
					timer->Bonus.timer = SDL_AddTimer(getConfigInt("bonus_removeinterval"), BonusFunction, NULL);
					timer->Bonus.run = 1;
					status->bonustimeleft = getConfigInt("bonus_time");
					Bonus3start(status, timer);
					break;
				case SDLK_b:	// exit bonus
				case SDLK_d:
				case SDLK_f:
					wombatlog("Event: exit bonus", 1);
					status->ifbonus=0;
					break;
			}
			break;
	}

	return 0;
}

int event_bonus1(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer)
{
	char text[30];
	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_z:
				case SDLK_x:
					wombatlog("Event: bonus1: leftarrow", 1);	
					status->bonus1active=0;
					break;
				case SDLK_c:
				case SDLK_v:
					wombatlog("Event: bonus1: rightarrow", 1);
					status->bonus1active=1;
					break;
				case SDLK_1:
					wombatlog("Event: bonus1: 1", 1);
					Bonus1Input(status, 0);
					break;
				case SDLK_w:
					wombatlog("Event: bonus1: 6", 1);
					Bonus1Input(status, 1);
					break;
				case SDLK_3:
					wombatlog("Event: bonus1: 3", 1);
					Bonus1Input(status, 2);
					break;
				case SDLK_b:	// exit bonus
				case SDLK_d:
				case SDLK_f:
					wombatlog("Event: exit bonus", 1);
					status->ifbonus=0;
					SDL_RemoveTimer(timer->Bonus.timer);
					timer->Bonus.run=0;
					status->bonusperformancesuspend = status->pbperformance;
					status->pbperformance = 0;
					status->bonustasktime += SDL_GetTicks()-status->bonusstarttime;
					break;
			}
			break;
		case SDL_JOYAXISMOTION:
			if (event->jaxis.which == Joy_Right_nr)
			{
				switch (event->jaxis.axis)
				{
					case 0:
						sprintf(text, "Event: JOY_X_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->x.joy = event->jaxis.value;
						break;
					case 1:
						sprintf(text, "Event: JOY_Y_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->y.joy = -event->jaxis.value;
						break;
				}
			}
			if (event->jaxis.which == Joy_Left_nr)
			{
				switch (event->jaxis.axis)
				{
					case 1:
						sprintf(text, "Event: JOY_Z_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->z.joy = event->jaxis.value;
						break;
				}
			}
			break;
	}

	return 0;
}


int event_bonus2(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_1:
					wombatlog("Event: bonus2: 1", 1);
					Bonus2Input(status, 1);
					break;
				case SDLK_2:
					wombatlog("Event: bonus2: 2", 1);
					Bonus2Input(status, 2);
					break;
				case SDLK_q:
					wombatlog("Event: bonus2: 3", 1);
					Bonus2Input(status, 3);
					break;
				case SDLK_w:
					wombatlog("Event: bonus2: 4", 1);
					Bonus2Input(status, 4);
					break;
				case SDLK_b:	// exit bonus
				case SDLK_d:
				case SDLK_f:
					wombatlog("Event: exit bonus", 1);
					status->ifbonus=0;
					SDL_RemoveTimer(timer->Bonus.timer);
					timer->Bonus.run=0;
					status->bonusperformancesuspend = status->pbperformance;
					status->pbperformance = 0;
					status->bonustasktime += SDL_GetTicks()-status->bonusstarttime;
					break;
			}
			break;
	}

	return 0;
}

int event_bonus3(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_1:
					wombatlog("Event: bonus3: 1", 1);
					Bonus3Input(status, timer, 1);
					break;
				case SDLK_2:
					wombatlog("Event: bonus3: 2", 1);
					Bonus3Input(status, timer, 2);
					break;
				case SDLK_3:
					wombatlog("Event: bonus3: 3", 1);
					Bonus3Input(status, timer, 3);
					break;
				case SDLK_4:
					wombatlog("Event: bonus3: 4", 1);
					Bonus3Input(status, timer, 4);
					break;
				case SDLK_q:
					wombatlog("Event: bonus3: 5", 1);
					Bonus3Input(status, timer, 5);
					break;
				case SDLK_w:
					wombatlog("Event: bonus3: 6", 1);
					Bonus3Input(status, timer, 6);
					break;
				case SDLK_e:
					wombatlog("Event: bonus3: 7", 1);
					Bonus3Input(status, timer, 7);
					break;
				case SDLK_r:
					wombatlog("Event: bonus3: 8", 1);
					Bonus3Input(status, timer, 8);
					break;
				case SDLK_b:	// exit bonus
				case SDLK_d:
				case SDLK_f:
					wombatlog("Event: exit bonus", 1);
					status->ifbonus=0;
					SDL_RemoveTimer(timer->Bonus.timer);
					timer->Bonus.run = 0;
					//status->bonus3st = 0;
					status->bonusperformancesuspend = status->pbperformance;
					status->pbperformance = 0;
					status->bonustasktime += SDL_GetTicks()-status->bonusstarttime;
					SDL_RemoveTimer(timer->Bonus31.timer);
					timer->Bonus31.run=0;
					SDL_RemoveTimer(timer->Bonus32.timer);
					timer->Bonus32.run=0;
					timer->Bonus32.timer=NULL;
					break;
			}
			break;
	}

	return 0;
}


int event_main(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer)
{
	char text[30];
	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_b:	// enter bonus
				case SDLK_d:
				case SDLK_f:
					wombatlog("Event: enter bonus", 1);
					status->ifbonus=1;
					if (status->whichbonus)
					{
						status->pbperformance = status->bonusperformancesuspend;
						timer->Bonus.timer = SDL_AddTimer(getConfigInt("bonus_removeinterval"), BonusFunction, NULL);
						timer->Bonus.run = 1;
					}
					if (status->whichbonus == 1)
						status->bonusstarttime = SDL_GetTicks();
					if (status->bonus1wfi == -1)
						status->bonus1wfi = 1;					
					if (status->whichbonus == 2)
						status->bonusstarttime = SDL_GetTicks();					
					if (status->whichbonus == 3)
					{
						status->bonusstarttime = SDL_GetTicks();
						Bonus3(status, timer);
					}
					break;
			}
			break;
		case SDL_JOYBUTTONDOWN:
			if (event->jaxis.which == Joy_Right_nr)
			{
				wombatlog("Event: JOYBUTTON", 1);
				if (event->jbutton.button == Joy_Right_button)
				{
					if (status->mode < 1)
					{
						wombatlog("Event: Run autotrack", 1);
						AutotrackRun(status, timer);
					}			
					else
					{
						wombatlog("Event: Stop autotrack", 1);
						AutotrackStop(status, timer);
					}
				}
			}
			break;
		case SDL_JOYAXISMOTION:
			if (event->jaxis.which == Joy_Right_nr)
			{
				switch (event->jaxis.axis)
				{
					case 0:
						sprintf(text, "Event: JOY_X_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->x.joy = event->jaxis.value;
						break;
					case 1:
						sprintf(text, "Event: JOY_Y_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->y.joy = -event->jaxis.value;
						break;
				}
			}
			if (event->jaxis.which == Joy_Left_nr)
			{
				switch (event->jaxis.axis)
				{
					case 1:
						sprintf(text, "Event: JOY_Z_AXIS: %i", event->jaxis.value);
						wombatlog(text, 1);
						status->z.joy = event->jaxis.value;
						break;
				}
			}
			break;

	}	
			
	return 0;
}

int event_menu(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer, ft_fontdata *fontdata, char *message)
{
	char text[60];

	switch (event->type)
	{
		case SDL_KEYDOWN:
			switch (event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					wombatlog("Event: menu ESC", 2);
					status->timeleft=0;
					break;
				case SDLK_s:
					wombatlog("Event: menu s", 1);
					status->ifmenu=0;
					break;
				case SDLK_j:
					wombatlog("Event: menu j", 1);
					joystick_setup_necessary(fontdata);
					joystick_setup(fontdata);
					break;
				case SDLK_l:
					wombatlog("Event: menu l", 1);
					configdefaultfilelocation(text, 60, "wombatsave", PACKAGE);
					if (wombatLoad(status, timer, text) == 0)
					{
						if (status->scenario_name[0] != '\0')
						{
							scenario_load_game_events(status->scenario_event_nr, status, timer);
						}
						strncpy(message, "Loaded", MESSAGE_SIZE);
					}
					else
						strncpy(message, "Error while loading", MESSAGE_SIZE);
					break;
				case SDLK_k:
					wombatlog("Event: menu k", 1);
					configdefaultfilelocation(text, 60, "wombatsave", PACKAGE);
					if (wombatSave(status, timer, text) == 0)
						strncpy(message, "Saved", MESSAGE_SIZE);
					else
						strncpy(message, "Error: not saved!", MESSAGE_SIZE);
					break;
			}
			break;
	}

	return 0;
}
					

