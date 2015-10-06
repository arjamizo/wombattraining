/***************************************************************************
 *            points.h
 *
 *  Fri Nov 20 00:26:48 2009
 *  Copyright  2009  bearh
 *  <bearh@<host>>
 ****************************************************************************/

#include "wombat.h"
#include "math.h"

typedef struct points_to_log {
	int lasttime;
	float autotrack;
	float bonus1;
	float bonus2;
	float bonus2solved;
	int bonus2howmanysolved;
	float bonus3;
	
	float autotrack_all;
	float bonus1_all;
	float bonus2_all;
	float bonus2solved_all;
	int bonus2howmanysolved_all;
	float bonus3_all;
	
	float from_lasttime_all;
	float prediction;
	float all;
} points_to_log;

int pointsCount(wombatGameStatus *status, int timeDelta);
int pointsPredict(wombatGameStatus *status);
int pointsMaintaskPerformanceUpdate(wombatGameStatus *status, int timeDelta);
int pointsMaintaskValueUpdate(wombatGameStatus *status);
int pointsBonusValueUpdate(wombatGameStatus *status, int nr);
int pointsBonusPerformanceUpdate(wombatGameStatus *status, int value);
int pointsAdd(wombatGameStatus *status, int value, int why);