#include "menu.h"

int menu_main(wombatGameStatus *status, wombatTimer *timer, ft_fontdata *fontdata, int *timeLast)
{
	SDL_Event event;
	int timeStart;
	char message[MESSAGE_SIZE];

	timeStart = SDL_GetTicks();
	
	status->ifmenu = 1;
	message[0] = '\0';
	
	wombatTimersSuspend(timer);

	while (status->timeleft && status->ifmenu)
	{
		while(SDL_PollEvent(&event))
		{
			event_menu(&event, status, timer, fontdata, message);
		}

		drawMenu(status, fontdata, message);
	}

	wombatTimersResume(timer, status);
	*timeLast += (SDL_GetTicks() - timeStart);
	
	return 0;
}