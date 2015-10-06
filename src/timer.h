#ifndef TIMER_H
	#define TIMER_H
#include <SDL.h>
#include <time.h>
#include "vars.h"
//#include "wombat.h"

	typedef struct wombatTimerElement {
		SDL_TimerID timer;
		short int run;
	} wombatTimerElement;

	typedef struct wombatTimer {
		wombatTimerElement AI;
		wombatTimerElement VeloAcce;
		wombatTimerElement AccelUpdate;
		wombatTimerElement Autotrack, AutotrackFailure, AutotrackRepair;
		wombatTimerElement Bonus;
		wombatTimerElement Bonus31, Bonus32;
	} wombatTimer;
	

Uint32 AITimerFunction(Uint32 interval, void *param);
Uint32 AutoTimerFunction(Uint32 interval, void *param);
Uint32 AutoFailureFunction(Uint32 interval, void *param);
Uint32 AutoRepairFunction(Uint32 interval, void *param);
Uint32 VeloAcceFunction(Uint32 interval, void *param);
Uint32 AccelUpdateFunction(Uint32 interval, void *param);
Uint32 BonusFunction(Uint32 interval, void *param);
Uint32 Bonus31Function(Uint32 interval, void *param);
Uint32 Bonus32Function(Uint32 interval, void *param);

#endif