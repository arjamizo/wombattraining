#include "vars.h"

/* Global variables */

int setDefaultConfig()
{
	config_createentry_int(&config[0], "screen_coldep", 8);
	config_createentry_int(&config[1], "screen_bpp", 32);

	config_createentry_int(&config[2], "joy1_max", 15000);
	config_createentry_int(&config[3], "joy1_min", 10);
	config_createentry_int(&config[4], "joy2_max", 30000);
	config_createentry_int(&config[5], "joy2_min", 10);
	config_createentry_int(&config[6], "joy1_vmax", 30);
	config_createentry_int(&config[7], "joy1_amax", 50);
	config_createentry_float(&config[8], "joy1_rotmax", 0.02);
	config_createentry_int(&config[9], "joy2_vmax", 30);
	config_createentry_int(&config[10], "joy2_amax", 50);
	config_createentry_float(&config[11], "joy2_rotmax", 0.02);
	config_createentry_int(&config[12], "joy1_accel_d", 10);
	config_createentry_int(&config[13], "joy2_accel_d", 12);

	config_createentry_int(&config[14], "cross_w", 25);
	config_createentry_int(&config[15], "cross_h", 25);
	config_createentry_int(&config[16], "circle_r", 50);
	config_createentry_int(&config[17], "circle_l", 50);
	config_createentry_int(&config[18], "lines_h", 20);
	config_createentry_int(&config[19], "lines_s", 10);
	config_createentry_int(&config[20], "joyind_scale", 40);
	config_createentry_int(&config[21], "joyind_r", 10);
	config_createentry_int(&config[22], "joyind_l", 10);

/*	config_createentry_int(&config[23], "fail_x", 0);
	config_createentry_int(&config[24], "fail_y", 600);
	config_createentry_int(&config[25], "fail_h", 20);
	config_createentry_int(&config[26], "fail_w", 60);
	config_createentry_int(&config[27], "vict_x", 740);
	config_createentry_int(&config[28], "vict_y", 580);
	config_createentry_int(&config[29], "vict_h", 20);
	config_createentry_int(&config[30], "vict_w", 60);
*/
	config_createentry_int(&config[31], "clock_r", 40);
	config_createentry_int(&config[32], "clock_l", 100);
	config_createentry_int(&config[33], "clock_x", 45);
	config_createentry_int(&config[34], "clock_y", 560);

	config_createentry_float(&config[35], "cross_cr", 1.0);
	config_createentry_float(&config[36], "cross_cg", 1.0);
	config_createentry_float(&config[37], "cross_cb", 0.0);
	config_createentry_float(&config[38], "circle_cr", 0.0);
	config_createentry_float(&config[39], "circle_cg", 0.0);
	config_createentry_float(&config[40], "circle_cb", 1.0);
	config_createentry_float(&config[41], "fail_cr", 1.0);
	config_createentry_float(&config[42], "fail_cg", 0.0);
	config_createentry_float(&config[43], "fail_cb", 0.0);
	config_createentry_float(&config[44], "vict_cr", 0.0);
	config_createentry_float(&config[45], "vict_cg", 1.0);
	config_createentry_float(&config[46], "vict_cb", 0.0);
	config_createentry_float(&config[47], "clock_b_cr", 0.2);
	config_createentry_float(&config[48], "clock_b_cg", 0.2);
	config_createentry_float(&config[49], "clock_b_cb", 0.2);
	config_createentry_float(&config[50], "clock_f_cr", 0.5);
	config_createentry_float(&config[51], "clock_f_cg", 0.5);
	config_createentry_float(&config[52], "clock_f_cb", 0.5);

	config_createentry_int(&config[163], "game_time", -1);
	config_createentry_int(&config[53], "probmax", 100);
	config_createentry_int(&config[54], "ai_timerinterval", 1000);
	config_createentry_int(&config[55], "ai_probx", 25);
	config_createentry_int(&config[56], "ai_proby", 25);
	config_createentry_int(&config[57], "ai_probz", 25);
	config_createentry_int(&config[58], "ai_posxmin", 50);
//	config_createentry_int(&config[59], "ai_posxmax", 750);
	config_createentry_int(&config[60], "ai_posymin", 50);
//	config_createentry_int(&config[61], "ai_posymax", 550);
	config_createentry_int(&config[62], "ai_poszmin", 50);
	config_createentry_int(&config[63], "ai_poszmax", 500);
	config_createentry_int(&config[64], "ai_posborder", 100);
	config_createentry_int(&config[65], "ai_borderspeedadd", 2);
	config_createentry_int(&config[66], "ai_speed_m", 5);
	config_createentry_int(&config[67], "ai_speed_d", 100);
	config_createentry_int(&config[68], "ai_speed_max", 10);
	config_createentry_int(&config[69], "ai_force_small_speed", 3);
	config_createentry_float(&config[70], "ai_force_speed_value", 1.5);

	config_createentry_int(&config[71], "at_updateinterval", 100);
	config_createentry_int(&config[72], "at_failureinterval", 5000);
	config_createentry_int(&config[73], "at_repairinterval", 3000);
	config_createentry_int(&config[74], "at_maxperformance", 100);
	config_createentry_int(&config[75], "at_fail_randmax", 110);
	config_createentry_int(&config[76], "at_fail_remove", 10);
	config_createentry_int(&config[77], "at_reducex", 500);
	config_createentry_int(&config[78], "at_reducey", 500);
	config_createentry_int(&config[79], "at_reducez", 100);
	config_createentry_int(&config[80], "at_repair_prob", 40);

	config_createentry_int(&config[81], "va_timerinterval", 5000);
	config_createentry_int(&config[82], "va_prob", 20);
	config_createentry_int(&config[83], "va_updateaccelinterval", 100);
	config_createentry_int(&config[162], "va_start", 1);

	config_createentry_int(&config[84], "bonus_removeinterval", 1000);
	config_createentry_int(&config[85], "bonus_time", 60);
	config_createentry_int(&config[86], "bonus_remove", 1);

	config_createentry_int(&config[87], "bonus1_rect_w", 25);
	config_createentry_int(&config[88], "bonus1_rect_h", 25);
	config_createentry_float(&config[89], "bonus1_fig_size", 3.0);
	config_createentry_int(&config[90], "bonus1_fig_nr", 0);

	config_createentry_int(&config[91], "bonus2_cols", 9);
	config_createentry_int(&config[92], "bonus2_rows", 6);
	config_createentry_int(&config[93], "bonus2_cell_w", 60);
	config_createentry_int(&config[94], "bonus2_cell_h", 60);
	config_createentry_int(&config[95], "bonus2_schemas", 0);
	config_createentry_int(&config[96], "bonus2_numbers", 32);
	config_createentry_int(&config[97], "bonus2_time_ok", 30000);
	config_createentry_int(&config[160], "bonus2_max_mistakes", 0);
	config_createentry_int(&config[98], "bonus2_rand_prob", 10);
	config_createentry_float(&config[99], "bonus2_n_cr", 0.9);
	config_createentry_float(&config[100], "bonus2_n_cg", 0.9);
	config_createentry_float(&config[101], "bonus2_n_cb", 0.9);

	config_createentry_int(&config[102], "bonus3_black_delay", 1000);
	config_createentry_int(&config[103], "bonus3_show_delay", 2000);
	config_createentry_int(&config[157], "bonus3_black_diff", 100);
	config_createentry_int(&config[158], "bonus3_black_max", 1500);
	config_createentry_int(&config[159], "bonus3_black_min", 500);
	config_createentry_int(&config[154], "bonus3_show_diff", 250);
	config_createentry_int(&config[155], "bonus3_show_max", 3000);
	config_createentry_int(&config[156], "bonus3_show_min", 1500);
	config_createentry_int(&config[169], "bonus3_hide_after_click", 0);
	config_createentry_int(&config[104], "bonus3_sq", 20);
	config_createentry_float(&config[105], "bonus3_sq_cr", 0.8);
	config_createentry_float(&config[106], "bonus3_sq_cg", 0.8);
	config_createentry_float(&config[107], "bonus3_sq_cb", 0.8);
	config_createentry_float(&config[108], "bonus3_f_cr", 0.2);
	config_createentry_float(&config[109], "bonus3_f_cg", 0.2);
	config_createentry_float(&config[110], "bonus3_f_cb", 0.2);
	
	config_createentry_int(&config[120], "points_border", 4);
	config_createentry_int(&config[121], "points_h", 100);
	config_createentry_int(&config[122], "points_w", 80);
	config_createentry_int(&config[131], "points_bar_w", 40);
	config_createentry_int(&config[132], "points_bar_h", 600);
	config_createentry_int(&config[133], "points_bar_max", 600);
	config_createentry_float(&config[165], "points_triangle_height", 2000.0);	// for 1 point/s
	
	config_createentry_float(&config[123], "points_m_cr", 0.0);
	config_createentry_float(&config[124], "points_m_cg", 0.0);
	config_createentry_float(&config[125], "points_m_cb", 1.0);
	config_createentry_float(&config[126], "points_b_cr", 1.0);
	config_createentry_float(&config[127], "points_b_cg", 0.0);
	config_createentry_float(&config[128], "points_b_cb", 1.0);
	config_createentry_float(&config[134], "points_bar_cr", 0.0);
	config_createentry_float(&config[135], "points_bar_cg", 1.0);
	config_createentry_float(&config[136], "points_bar_cb", 0.0);
	
	config_createentry_int(&config[129], "points_performance_max", 100);
	config_createentry_int(&config[130], "points_value_max", 100);
	config_createentry_int(&config[119], "points_D", 1000);
	
	config_createentry_int(&config[137], "points_rjoy_performance", 66);
	config_createentry_int(&config[138], "points_rjoy_maxdist", 100);
	config_createentry_int(&config[139], "points_ljoy_maxdist", 50);

	config_createentry_int(&config[140], "points_mvalue_tb", 15000); // time of last bonus play
	config_createentry_float(&config[143], "points_mvalue_tb_remove", 1.0); //every second not playing bonus remove this value
	config_createentry_int(&config[141], "points_mvalue_at_e", 100); // autotrack failure efficiency
	config_createentry_float(&config[144], "points_mvalue_at_e_add", 2); // when finding failure earilier than last time, status->pmvate is bigger (difference / this)
	config_createentry_int(&config[142], "points_mvalue_accvelo_diff", 20); // difference between accel/velo mode
	config_createentry_float(&config[145], "points_mvalue_eff_dps", 1.0); // efficiency drop per second
	config_createentry_float(&config[146], "points_mvalue_eff_diff_m", 0.1); // multiple by this the difference between efficiency

	config_createentry_int(&config[147], "points_bvalue_remove", 10);	// how many remove from value when choose bonus
	config_createentry_int(&config[148], "points_b1p_timeok", 15000);	// how long performance max
	config_createentry_float(&config[149], "points_b1p_remove", 7);	// how many remove when time is longer (every second)
	config_createentry_int(&config[150], "points_b2p_value", 3);	// 
	config_createentry_int(&config[161], "points_b2p_newschema", 10);
	config_createentry_int(&config[151], "points_b3p_value", 3);	// 
	config_createentry_int(&config[152], "points_b3p_timeok", 2000);	// how long performance not changing
	config_createentry_float(&config[153], "points_b3p_timeremove", 5.0);	// how many remove when time is longer (every second)

	config_createentry_int(&config[164], "points_prediction_time", 60);		// how long remember points for prediction
	config_createentry_float(&config[166], "points_triangle_timeD", 4.0);	// how many times is shorter time to draw triangle

	config_createentry_string(&config[167], "points_log_file", "wombat_points");
	config_createentry_int(&config[168], "points_log_interval", 60);	// in seconds how often log to file
	
	config_createentry_int(&config[111], "loglevel", 5);

	config_createentry_int(&config[112], "screen_w", 800);
	config_createentry_int(&config[113], "screen_h", 600);
	config_createentry_int(&config[114], "taskarea_w", 800);
	config_createentry_int(&config[115], "taskarea_h", 600);

	config_createentry_string(&config[116], "font_path", "/usr/share/fonts/corefonts/cour.ttf");
	config_createentry_int(&config[117], "font_size", 16);
	config_createentry_int(&config[118], "font_dpi", 96);

	return 0;
}

int saveConfig()
{
	char filename[WOMBAT_FILENAME_SIZE];

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "config", PACKAGE);
	configcreatedirforfile(filename);
	configwrite(filename ,config, WOMBAT_CONFIG_ENTRIES);

	return 0;
}

int loadConfig()
{
	char filename[WOMBAT_FILENAME_SIZE];

#ifdef DATADIR2

	strncpy(filename, DATADIR, WOMBAT_FILENAME_SIZE);
	strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, PACKAGE, WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, "/config", WOMBAT_FILENAME_SIZE-strlen(filename));
	configread(filename, config, WOMBAT_CONFIG_ENTRIES);

#endif
	
	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "config", PACKAGE);
	configread(filename, config, WOMBAT_CONFIG_ENTRIES);

	if (SCENARIO_PATH[0] != '\0')
	{
		strncpy(filename, SCENARIO_PATH, WOMBAT_FILENAME_SIZE);
		strncat(filename, "config", WOMBAT_FILENAME_SIZE-strlen(filename));
		if (configread(filename, config, WOMBAT_CONFIG_ENTRIES) == 0)
		{
			wombatlog("Read config for scenario", 2);
		}
		else {
			wombatlog("Problem loading config for scenario, loading default", 2);
		}
	}


	return 0;
}

int updateConfigInt(char *name, int value)
{
	return config_updateentry_int(config, WOMBAT_CONFIG_ENTRIES, name, value);
}
int updateConfigFloat(char *name, float value)
{
	return config_updateentry_float(config, WOMBAT_CONFIG_ENTRIES, name, value);
}
int getConfigInt(char *name)
{
	return config_getentry_int(config, WOMBAT_CONFIG_ENTRIES, name);
}
float getConfigFloat(char *name)
{
	return config_getentry_float(config, WOMBAT_CONFIG_ENTRIES, name);
}
char *getConfigString(char *name)
{
	return config_getentry_string(config, WOMBAT_CONFIG_ENTRIES, name);
}
	

int setGlobalVars()
{
	updateConfigInt("taskarea_w", getConfigInt("screen_w")-getConfigInt("points_w"));
	updateConfigInt("taskarea_h", getConfigInt("screen_h")-getConfigInt("points_h"));

	updateConfigInt("clock_x", getConfigInt("screen_w")-getConfigInt("clock_x"));
	updateConfigInt("clock_y", getConfigInt("screen_h")-getConfigInt("clock_r")-5);

	return 0;
}

int copyConfig2Vars()
{
//	SCREEN_W = getConfigInt("screen_w");
//	SCREEN_H = getConfigInt("screen_h");
//	SCREEN_COLDEP = getConfigInt("screen_coldep");
//	SCREEN_BPP = getConfigInt("screen_bpp");
	
	/* joystick */
//	JOY1MAX = getConfigInt("joy1_max");
//	JOY1MIN = getConfigInt("joy1_min");	
//	JOY2MAX = getConfigInt("joy2_max");
//	JOY2MIN = getConfigInt("joy2_min");
//	JOY1VMAX = getConfigInt("joy1_vmax");	// velocity when JOY reaches MAX
//	JOY1AMAX = getConfigInt("joy1_amax");
//	JOY1ROTMAX = getConfigFloat("joy1_rotmax");		// max rotation in bonus1
//	JOY2VMAX = getConfigInt("joy2_vmax");		// velocity when JOY reaches MAX   //10
//	JOY2AMAX = getConfigInt("joy2_amax");		//
//	JOY2ROTMAX = getConfigFloat("joy2_rotmax");		// max rotation in bonus1
//	JOY1AccelD = getConfigInt("joy1_accel_d");	// variable in cat accel equation
//	JOY2AccelD = getConfigInt("joy2_accel_d");	// variable in cat accel equation
	
	/* main display sizes */
/*	TASKAREA_W = getConfigInt("taskarea_w");	// main task area could be smaller than screen
	TASKAREA_H = getConfigInt("taskarea_h");	
	CROSS_W = getConfigInt("cross_w");	// cross width
	CROSS_H = getConfigInt("cross_h");	// cross height
	SQ_R = getConfigInt("circle_r");		// circle radius (originally it was a squere instead of circle)
	SQ_L = getConfigInt("circle_l");		// number of lines to draw circle
	LINES_H = getConfigInt("lines_h");	// horizontal lines height
	LINES_S = getConfigInt("lines_s");	// horizontal lines separation		//20
	JOYSCALE = getConfigInt("joyind_scale");	// maximum length of joystick indicator
	JOYI_R = getConfigInt("joyind_r");		// joystick indicator radius
	JOYI_L = getConfigInt("joyind_l");		// number of lines to draw indicator
*/	
/*	FAIL_X = getConfigInt("fail_x");
	FAIL_Y = getConfigInt("fail_y");
	FAIL_H = getConfigInt("fail_h");
	FAIL_W = getConfigInt("fail_w");
	VICT_X = getConfigInt("vict_x");
	VICT_Y = getConfigInt("vict_y");
	VICT_H = getConfigInt("vict_h");
	VICT_W = getConfigInt("vict_w");
*/
/*	CLOCK_X = getConfigInt("clock_x");
	CLOCK_Y = getConfigInt("clock_y");	
	CLOCK_R = getConfigInt("clock_r");	//30
	CLOCK_L = getConfigInt("clock_l");
*/	
	/* main display colours */
/*	CROSS_CR = getConfigFloat("cross_cr");	// cat rgb
	CROSS_CG = getConfigFloat("cross_cg");
	CROSS_CB = getConfigFloat("cross_cb");
	SQ_CR = getConfigFloat("circle_cr");	// mouse rgb
	SQ_CG = getConfigFloat("circle_cg");
	SQ_CB = getConfigFloat("circle_cb");
*/	
/*	FAIL_CR = getConfigFloat("fail_cr");
	FAIL_CG = getConfigFloat("fail_cg");
	FAIL_CB = getConfigFloat("fail_cb");			//40
	VICT_CR = getConfigFloat("vict_cr");
	VICT_CG = getConfigFloat("vict_cg");
	VICT_CB = getConfigFloat("vict_cb");
*/	
/*	CLOCK_B_CR = getConfigFloat("clock_b_cr");
	CLOCK_B_CG = getConfigFloat("clock_b_cg");
	CLOCK_B_CB = getConfigFloat("clock_b_cb");
	CLOCK_F_CR = getConfigFloat("clock_f_cr");
	CLOCK_F_CG = getConfigFloat("clock_f_cg");
	CLOCK_F_CB = getConfigFloat("clock_f_cb");
*/	
	/* main task AI settings*/
/*	AITimerInterval = getConfigInt("ai_timerinterval");	// how often change velocity of mouse (miliseconds)	//50
	AIProbMax = getConfigInt("probmax");		// maximum probability (100%) 
	AIProbX = getConfigInt("ai_probx");		// probability of changing velocity
	AIProbY = getConfigInt("ai_proby");
	AIProbZ = getConfigInt("ai_probz");
	AIPosXMin = getConfigInt("ai_posxmin");					// mouse position edges
	AIPosXMax = getConfigInt("ai_posxmax");	
	AIPosYMin = getConfigInt("ai_posymin");
	AIPosYMax = getConfigInt("ai_posymax");
	AIPosZMin = getConfigInt("ai_poszmin");
	AIPosZMax = getConfigInt("ai_poszmax");
	AIPosBorder = getConfigInt("ai_posborder");		// place where mouse slows down not to hit edges
	AIBorderSpeedAdd = getConfigInt("ai_borderspeedadd");// ratio of decrement speed being in border area		//60
	AISpeedM = getConfigInt("ai_speed_m");			//numbers used in velocity equation
	AISpeedD = getConfigInt("ai_speed_d");
	AISpeedMax = getConfigInt("ai_speed_max");	// never exceeded speed
	AIForceSmallSpeed = getConfigInt("ai_force_small_speed"); // when velocity is smaller multiple its value
	AIForceSpeedValue  = getConfigFloat("ai_force_speed_value"); // using this number
*/	
	/* autotrack */
/*	AutoTimerInterval = getConfigInt("at_updateinterval");		// how often update velocity
	AutoTimerFailure = getConfigInt("at_failureinterval");		// how often checks for failure
	AutoRepairInterval = getConfigInt("at_repairinterval");	//how often checks for reparation
	AutoMaxPerformance = getConfigInt("at_maxperformance");
	AutoFailRandMax = getConfigInt("at_fail_randmax");// after timer interval checks if random number is higher than performance, if so:  //70
	AutoFailRemove = getConfigInt("at_fail_remove");			// removes this nr from performance
	AutoReduceX = getConfigInt("at_reducex");
	AutoReduceY = getConfigInt("at_reducey");
	AutoReduceZ = getConfigInt("at_reducez");
	AutoRepairProb = getConfigInt("at_repair_prob");		// probability of repairing Autotrack (max in AI settings)
*/	
	/* Velo / Accel */
/*	VATimerInterval = getConfigInt("va_timerinterval");
	VAProb = getConfigInt("va_prob");
	AccelUpdateInterval = getConfigInt("va_updateaccelinterval");
*/	
	/* Bonus */
/*	BonusInterval = getConfigInt("bonus_removeinterval");		// remove time: how often
	BonusTime = getConfigInt("bonus_time");				// first value of time		//80
	BonusTimeRemove = getConfigInt("bonus_remove");		// how many remove
*/	
	/* Bonus1 */
/*	Bonus1RectW = getConfigInt("bonus1_rect_w");
	Bonus1RectH = getConfigInt("bonus1_rect_h");
	Bonus1FigSize = getConfigFloat("bonus1_fig_size");
	Bonus1Figs = getConfigInt("bonus1_fig_nr");
*/	
	/* Bonus2 */
/*	Bonus2Cols = getConfigInt("bonus2_cols");
	Bonus2Rows = getConfigInt("bonus2_rows");
	Bonus2CellW = getConfigInt("bonus2_cell_w");
	Bonus2CellH = getConfigInt("bonus2_cell_h");
	Bonus2Schemas = getConfigInt("bonus2_schemas");		// how many schemas		//90
	Bonus2Numbers = getConfigInt("bonus2_numbers");		// how many numbers to find
	Bonus2TimeOK = getConfigInt("bonus2_time_ok");		// if finished faster than ..., use next schema
	Bonus2RandProb = getConfigInt("bonus2_rand_prob");			// probability of using random schema
	Bonus2N_CR = getConfigFloat("bonus2_n_cr");
	Bonus2N_CG = getConfigFloat("bonus2_n_cg");
	Bonus2N_CB = getConfigFloat("bonus2_n_cb");
*/	
	/* Bonus3 */
/*	Bonus3BlackDelay = getConfigInt("bonus3_black_delay");
	Bonus3ShowDelay = getConfigInt("bonus3_show_delay");
	Bonus3SQ = getConfigInt("bonus3_sq");
	Bonus3SQ_CR = getConfigFloat("bonus3_sq_cr");		//100
	Bonus3SQ_CG = getConfigFloat("bonus3_sq_cg");
	Bonus3SQ_CB = getConfigFloat("bonus3_sq_cb");
	Bonus3F_CR = getConfigFloat("bonus3_f_cr");
	Bonus3F_CG = getConfigFloat("bonus3_f_cg");
	Bonus3F_CB = getConfigFloat("bonus3_f_cb");

	WombatLogLevel = getConfigInt("loglevel");	//106
*/
	
	return 0;
}
