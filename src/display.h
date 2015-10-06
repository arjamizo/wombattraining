#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "wombat.h"
#include "vars.h"
#include "font.h"
#include "logging.h"
#include "configfile.h"
#include "bonus1fig.h"

int initDisplay(SDL_Surface *screen);
int drawGLScene(wombatGameStatus *status, ft_fontdata *fontdata);
int drawMainTask(wombatGameStatus *status);
int drawTopScreen(wombatGameStatus *status, ft_fontdata *fontdata);
int drawFPS(wombatGameStatus *status, ft_fontdata *fontdata);
int drawBonus(wombatGameStatus *status, ft_fontdata *fontdata);
int drawMenu(wombatGameStatus *status, ft_fontdata *fontdata, char *message);
int drawBonusValueRect(wombatGameStatus *status, int top, int value);
int drawSelectBonus(wombatGameStatus *status, ft_fontdata *fontdata);
int drawClock(wombatGameStatus *status);
void tcbVertex(GLvoid *vertex);
int drawBonus1(wombatGameStatus *status, ft_fontdata *fontdata);
int drawBonus2(wombatGameStatus *status, ft_fontdata *fontdata);
int drawBonus3(wombatGameStatus *status, ft_fontdata *fontdata);
