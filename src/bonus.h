#ifndef BONUS_H
# define BONUS_H
#include "font.h"
#include "timer.h"
#include "wombat.h"
#include "logging.h"
#include "vars.h"

#include <SDL.h>
#include <SDL_opengl.h>

#endif

int BonusRemoveTime(wombatGameStatus *status, wombatTimer *timer);
int Bonus1start(wombatGameStatus *status);
int Bonus1Input(wombatGameStatus *status, int nr);
int Bonus2start(wombatGameStatus *status);
int Bonus2Input(wombatGameStatus *status, int rectangle);
int Bonus3start(wombatGameStatus *status, wombatTimer *timer);
int Bonus3(wombatGameStatus *status, wombatTimer *timer);
int Bonus31(wombatGameStatus *status, wombatTimer *timer);
int Bonus32(wombatGameStatus *status, wombatTimer *timer);
int Bonus3Input(wombatGameStatus *status, wombatTimer *timer, int nr);
int Bonus3ChangeDelay(wombatGameStatus *status, int showvalue, int blackvalue);
