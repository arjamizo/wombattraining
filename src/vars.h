#include "configfile.h"

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#ifndef PACKAGE
	#define PACKAGE wombattraining
#endif

#ifndef VARS_H
	#define VARS_H
	
	#define WOMBAT_CONFIG_ENTRIES 170
	#define WOMBAT_FILENAME_SIZE 80

	configentry config[WOMBAT_CONFIG_ENTRIES];
	char SCENARIO_PATH[WOMBAT_FILENAME_SIZE];

	/* global vars  //new - not config*/ 

	int Joy_Right_nr;
	int Joy_Right_button;
//	int Joy_Right_max;
//	int Joy_Right_min;
	int Joy_Left_nr;
	int Joy_Left_button;
//	int Joy_Left_max;
//	int Joy_Left_min;

	/* screen */
/*	int SCREEN_W;
	int SCREEN_H;
	int SCREEN_COLDEP;
	int SCREEN_BPP;
*/	
	/* joystick */
/*	int JOY1MAX;
	int JOY1MIN;	
	int JOY2MAX;
	int JOY2MIN;
	int JOY1VMAX;	// velocity when JOY reaches MAX
	int JOY1AMAX;	//
	float JOY1ROTMAX;		// max rotation in bonus1
	int JOY2VMAX;		// velocity when JOY reaches MAX   //10
	int JOY2AMAX;		//
	float JOY2ROTMAX;		// max rotation in bonus1
	int JOY1AccelD;	// variable in cat accel equation
	int JOY2AccelD;	// variable in cat accel equation
*/	
	/* main display sizes */
/*	int TASKAREA_W;	// main task area could be smaller than screen
	int TASKAREA_H;	
	int CROSS_W;	// cross width
	int CROSS_H;	// cross height
	int SQ_R;		// circle radius (originally it was a squere instead of circle)
	int SQ_L;		// number of lines to draw circle
	int LINES_H;	// horizontal lines height
	int LINES_S;	// horizontal lines separation		//20
	int JOYSCALE;	// maximum length of joystick indicator
	int JOYI_R;		// joystick indicator radius
	int JOYI_L;		// number of lines to draw indicator
*/	
/*	int FAIL_X;
	int FAIL_Y;
	int FAIL_H;
	int FAIL_W;
	int VICT_X;
	int VICT_Y;
	int VICT_H;
	int VICT_W;
*/	
/*	int CLOCK_X;
	int CLOCK_Y;	
	int CLOCK_R;	//30
	int CLOCK_L;
*/	
	/* main display colours */
/*	int CROSS_CR;	// cat rgb
	int CROSS_CG;
	int CROSS_CB;
	int SQ_CR;	// mouse rgb
	int SQ_CG;
	int SQ_CB;
*/	
/*	int FAIL_CR;
	int FAIL_CG;
	int FAIL_CB;			//40
	float VICT_CR;
	int VICT_CG;
	float VICT_CB;
*/	
/*	float CLOCK_B_CR;
	float CLOCK_B_CG;
	float CLOCK_B_CB;
	float CLOCK_F_CR;
	float CLOCK_F_CG;
	float CLOCK_F_CB;
*/	
	/* main task AI settings*/
/*	int AITimerInterval;	// how often change velocity of mouse (miliseconds)	//50
	int AIProbMax;		// maximum probability (100%) 
	int AIProbX;		// probability of changing velocity
	int AIProbY;
	int AIProbZ;
	int AIPosXMin;					// mouse position edges
	int AIPosXMax;	
	int AIPosYMin;
	int AIPosYMax;
	int AIPosZMin;
	int AIPosZMax;
	int AIPosBorder;		// place where mouse slows down not to hit edges
	int AIBorderSpeedAdd;// ratio of decrement speed being in border area		//60
	int AISpeedM;			//numbers used in velocity equation
	int AISpeedD;
	int AISpeedMax;	// never exceeded speed
	int AIForceSmallSpeed; // when velocity is smaller multiple its value
	float AIForceSpeedValue ; // using this number
*/	
	/* autotrack */
/*	int AutoTimerInterval;		// how often update velocity
	int AutoTimerFailure;		// how often checks for failure
	int AutoRepairInterval;	//how often checks for reparation
	int AutoMaxPerformance;
	int AutoFailRandMax;// after timer interval checks if random number is higher than performance, if so:  //70
	int AutoFailRemove;			// removes this nr from performance
	int AutoReduceX;
	int AutoReduceY;
	int AutoReduceZ;
	int AutoRepairProb;		// probability of repairing Autotrack (max in AI settings)
*/	
	/* Velo / Accel */
/*	int VATimerInterval;
	int VAProb;
	int AccelUpdateInterval;
*/	
	/* Bonus */
/*	int BonusInterval;		// remove time: how often
	int BonusTime;				// first value of time		//80
	int BonusTimeRemove;		// how many remove
*/	
	/* Bonus1 */
/*	int Bonus1RectW;
	int Bonus1RectH;
	float Bonus1FigSize;
	int Bonus1Figs;
*/	
	/* Bonus2 */
//#define Bonus2Cols 9
//#define Bonus2Rows 6
//#define Bonus2Schemas 11
//#define Bonus2Numbers 32
/*	int Bonus2Cols;
	int Bonus2Rows;
	int Bonus2CellW;
	int Bonus2CellH;
	int Bonus2Schemas;		// how many schemas		//90
	int Bonus2Numbers;		// how many numbers to find
	int Bonus2TimeOK;		// if finished faster than ..., use next schema
	int Bonus2RandProb;			// probability of using random schema
	float Bonus2N_CR;
	float Bonus2N_CG;
	float Bonus2N_CB;
*/	
	/* Bonus3 */
/*	int Bonus3BlackDelay;
	int Bonus3ShowDelay;
	int Bonus3SQ;
	float Bonus3SQ_CR;		//100
	float Bonus3SQ_CG;
	float Bonus3SQ_CB;
	float Bonus3F_CR;
	float Bonus3F_CG;
	float Bonus3F_CB;

	int WombatLogLevel;	//106
*/
#endif


int setDefaultConfig();
int saveConfig();
int loadConfig();
int updateConfigInt(char *name, int value);
int updateConfigFloat(char *name, float value);
int getConfigInt(char *name);
float getConfigFloat(char *name);
char *getConfigString(char *name);
int setGlobalVars();
int copyConfig2Vars();
