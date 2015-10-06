#ifndef EVENTS_H
#define EVENTS_H

#include "wombat.h"
#include "timer.h"
#include "bonus.h"
#include "logging.h"
#include "menu.h"
#include "scenario.h"
#include <SDL.h>

int event_handler(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer, ft_fontdata *fontdata, int *timeLast);
int event_bonus0(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer);
int event_bonus1(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer);
int event_bonus2(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer);
int event_bonus3(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer);
int event_main(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer);
int event_menu(SDL_Event *event, wombatGameStatus *status, wombatTimer *timer, ft_fontdata *fontdata, char *message);


#endif
