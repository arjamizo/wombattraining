#ifndef WOMBAT_H
#define WOMBAT_H

#include "vars.h"
#include "timer.h"
#include "logging.h"
#include "bonus1fig.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VERSION_SIZE 20
#define POINTS_TABLE_SIZE 180

	typedef struct wombatMainPart {
		float cat, m;
		int joy;		// positions of cat and mouse
	  	float mv, catv;		// joystick position, mouse velocity, cat velocity
		
		int aimprob;		//probability of change velocity
		int mposmin, mposmax;	// mouse position edges
		int taskarea;		// area for the element
		int autoreduce;	// autotrack reduce speed value
	} wombatMainPart;

	typedef struct wombatGameStatus {
		float timeleft;			// to the end of the game (-1 forever)
		wombatMainPart x, y, z;	// three main parts in wombat
		short int mode, failure;			// -1:velocity control; 0:accel control; more: autotrack
		
		short int ifmenu;

		int at_timelast;

		short int ifbonus;			// flag if the bonus is on
		short int whichbonus;		// 1: 3d shapes; 2: find numbers; 3: remember digits
		int bonustimeleft;
		int bonustasktime;
		//int bonusresult;		// 1: victory; 0: nothing; -1: failure
		unsigned int bonusstarttime;
		int bonusperformancesuspend;
		unsigned int bonusexittime;
		int bonusvalue[3];
		
		short int bonus1active;		// 0: left; 1:right
		unsigned int bonus1fig1nr, bonus1fig2nr;
		short int bonus1mode;		// 0: the same; 1: symetric; 2:different; 3:different, symetric
		short int bonus1wfi;			// wait for input
		float bonus11rotx;
		float bonus11roty;
		float bonus11rotz;
		float bonus12rotx;
		float bonus12roty;
		float bonus12rotz;
		
		unsigned int bonus2snr;			// bonus 2 schema number
		//int bonus2starttime;		// moved to bonusstrattime (all bonuses requires starttime for points counting)
		unsigned short int bonus2whichnr;	// how many correct nrs already  		
		unsigned short int bonus2table[32][3];		// for every single number: which rectangle, col and row nr;
		unsigned short int bonus2rand;	   // flag if next schema should be random one
		unsigned int bonus2mistakes;
		
		unsigned short int bonus3dlts;			// digits left to show
		unsigned short int bonus3d[3];			// generated digits			
		unsigned short int bonus3digit;				// status: 1-8: digit to show; 0 - show nothing
		unsigned short int bonus3wfi;			// if wait for input
		unsigned int bonus3showdelay;
		unsigned int bonus3blackdelay;

		float ppoints, ppredicted;		// points counter
		int pmperformance, pmvalue;		// main task performance, value indicator
		int pbperformance, pbvalue;		// bonus performance, value indicator
		float pmvaccvelo, pmvaccat;		// main task value acceleration/velocity difference; acc autotrack difference
		float pmvate;		// m autotrack failure notice efficiency
		float pacce, pveloe;		// acceleration/velocity mode tracking efficiency
		float ptable[POINTS_TABLE_SIZE][2];	// store values for counting points and score prediction

		char scenario_name[WOMBAT_FILENAME_SIZE];
		unsigned int scenario_event_nr;
	} wombatGameStatus;

int wombatGameStatusReset(wombatGameStatus *status);
int mainTaskUpdatePos(wombatGameStatus *status, int timeDelta);
int mainTaskAI(wombatMainPart *element);
int mainTaskAuto(wombatMainPart *element, int performance, int timeDelta);
int AutotrackRun(wombatGameStatus *status, wombatTimer *timer);
int AutotrackStop(wombatGameStatus *status, wombatTimer *timer);
int AutoConditions(wombatGameStatus *status);
int AutoFail(wombatGameStatus *status, wombatTimer *timer);
int AutoRepair(wombatGameStatus *status, wombatTimer *timer);
//int JoystickHandler(wombatMainPart *element, int mode, int axis);
int VeloAcce(wombatGameStatus *status, wombatTimer *timer);
int VeloAcceChange(wombatGameStatus *status, wombatTimer *timer);
int AccelUpdate(wombatGameStatus *status);
int wombatSave(wombatGameStatus *status, wombatTimer *timer, char *filename);
int wombatLoad(wombatGameStatus *status, wombatTimer *timer, char *filename);

int UpdateStatusParameter(int nr, int value, wombatGameStatus *status, wombatTimer *timer);

int wombatTimersStart(wombatTimer *timer);
int wombatTimersSuspend(wombatTimer *timer);
int wombatTimersResume(wombatTimer *timer, wombatGameStatus *status);
int wombatTimerSuspend(wombatTimerElement *id);
int wombatTimerResume(wombatTimerElement *id, Uint32 interval, SDL_NewTimerCallback callback);

#endif
