#include "timer.h"


Uint32 AITimerFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 AutoTimerFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 1;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 AutoFailureFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 2;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 AutoRepairFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 3;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 VeloAcceFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 4;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}


Uint32 AccelUpdateFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 5;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 BonusFunction(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 6;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 Bonus31Function(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 7;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}

Uint32 Bonus32Function(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;
	
	userevent.type = SDL_USEREVENT;
	userevent.code = 8;
	userevent.data1 = NULL;
	userevent.data2 = NULL;
	
	event.type = SDL_USEREVENT;
	event.user = userevent;
	
	SDL_PushEvent(&event);
	return interval;
}