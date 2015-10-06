#include "scenario.h"

static scenario_event event;

int scenario_check_name(char *scenario_name)
{
	char filename[WOMBAT_FILENAME_SIZE];

#ifdef DATADIR

	strncpy(filename, DATADIR, WOMBAT_FILENAME_SIZE);
	strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, PACKAGE, WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, "/scenarios/", WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, scenario_name, WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));

	if (scenario_check_path(filename) == 0)
	{
		strncpy(SCENARIO_PATH, filename, WOMBAT_FILENAME_SIZE);
		return 0;
	}
	else {
		SCENARIO_PATH[0] = '\0';
	}

#endif
	
	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "scenarios/", PACKAGE);
	strncat(filename, scenario_name, WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));
	
	if (scenario_check_path(filename) == 0)
	{
		strncpy(SCENARIO_PATH, filename, WOMBAT_FILENAME_SIZE);
		return 0;
	}
	else {
		SCENARIO_PATH[0] = '\0';
	}

	return -1;
}

int scenario_check_path(char *scenario_path)
{
	struct stat *scenariofile;
	
	wombatlog("Allocating memory for scenario file status", 2);
	scenariofile = (struct stat *)malloc(sizeof(struct stat));	
	if (scenariofile == NULL)
	{
		wombatlog("Not enough memory for scenario file", 10);
		return -1;
	}

	if (stat (scenario_path, scenariofile) != 0)
	{
		wombatlog("Probably wrong path to scenario", 5);
		free(scenariofile);
		return -1;
	} 

	if (S_ISDIR(scenariofile->st_mode))
	{
		if (access(scenario_path, R_OK | X_OK) != 0)
		{
			wombatlog("Permission denied for scenario directory", 10);
			free(scenariofile);
			return -1;
		}
		else {
			wombatlog("Found scenario directory", 2);
		}
	}

	free(scenariofile);
	
	return 0;
}

int scenario_load_event(unsigned int nr)
{
	FILE *events_file;
	char filename[WOMBAT_FILENAME_SIZE];
	char buff[SCENARIO_EVENT_BUFFSIZE];
	unsigned int rownr=0, i=0;

	wombatlog("Check scenario path", 1);
	if (SCENARIO_PATH[0] == '\0')
	{
		wombatlog("Loading scenario event while no scenario loaded?", 5);
		return -1;
	}

	strncpy(filename, SCENARIO_PATH, WOMBAT_FILENAME_SIZE);
	strncat(filename, "scenario_events", WOMBAT_FILENAME_SIZE-strlen(filename));

	events_file = fopen(filename, "r");

	if (events_file == NULL)
	{
		wombatlog("Can't open scenario events file", 5);
		event.when=-1;
		event.what=0;
		event.parameter=0;
		return -1;
	}

	while (rownr < nr)
	{
		if (feof(events_file))
		{
			wombatlog("Reached end of scenario event file too early", 5);
			event.when=-1;
			event.what=0;
			event.parameter=0;
			return -1;
		}
		if (fgetc(events_file) == '\n')
		{
			rownr++;
		}
	}

	while (((buff[i] = fgetc(events_file)) != '\n') && (i < SCENARIO_EVENT_BUFFSIZE))
	{
		i++;
		if (feof(events_file))
		{
			wombatlog("Reached end of scenario event file too early2", 5);
			event.when=-1;
			event.what=0;
			event.parameter=0;
			return -1;
		}
	}

	buff[i] = '\0';

	if (sscanf(buff, "%i:%i:%i", &(event.when), &(event.what), &(event.parameter)) < 3)
	{
		wombatlog("wrong line in scenario event file", 5);
		event.when=-1;
		event.what=0;
		event.parameter=0;
		return -1;
	}

	return 0;
}

int scenario_load_game_events(unsigned int nr, wombatGameStatus *status, wombatTimer *timer)
{
	int i;

	for (i=0; i <= nr; i++)
	{
		scenario_load_event (i);
		if (event.what > 199 && event.what < 300)
		{
			scenario_event_exe(status, timer);
		}
	}

	return 0;
}

int scenario_event_check(int time, unsigned int nr, wombatGameStatus *status, wombatTimer *timer)
{
	if (time < event.when)
	{
		scenario_event_exe(status, timer);
		if (scenario_load_event(nr+1) != 0)
		{
			return -1;
		}
		return 1;
	}

	return 0;
}

int scenario_event_exe(wombatGameStatus *status, wombatTimer *timer)
{


	 switch (event.what)
	 {
		case 1:
			printf("Scenario event: %i:%i:%i\n", event.when, event.what, event.parameter);
			break;
		case 10:		// autotrack failure
				 AutoFail(status, timer);
			 break;
		case 11:		// autotrack repair
				 AutoRepair(status, timer);
			 break;
		case 15:		// velo/acce change
				 VeloAcceChange(status, timer);
			 break;
			 
		case 110:		// mouse x pos
		case 111:		// mouse y pos
		case 112:		// mouse z pos
		case 113:		// mouse x vel
		case 114:		// mouse y vel
		case 115:		// mouse z vel
		case 116:		// aimprob x
		case 117:		// aimprob y
		case 118:		// aimprob z
		case 119:		// mouse min x pos
		case 120:		// mouse max x pos
		case 121:		// mouse min y pos
		case 122:		// mouse max y pos
		case 123:		// mouse min z pos
		case 124:		// mouse max z pos
		case 131:		// bonus2nr
			 UpdateStatusParameter(event.what, event.parameter, status, timer);
			 break;

		case 201:
			 updateConfigInt("at_repair_prob", event.parameter);
			 break;
		case 202:
			 updateConfigInt("ai_posborder", event.parameter);
			 break;
		case 203:
			 updateConfigInt("ai_borderspeedadd", event.parameter);
			 break;
		case 204:
			 updateConfigInt("ai_speed_m", event.parameter);
			 break;
		case 205:
			 updateConfigInt("ai_speed_max", event.parameter);
			 break;
		case 206:
			 updateConfigInt("ai_force_small_speed", event.parameter);
			 break;
		case 207:
			 updateConfigInt("at_fail_remove", event.parameter);
			 break;
		case 208:
			 updateConfigInt("va_prob", event.parameter);
			 break;
		case 209:
			 updateConfigInt("va_start", event.parameter);
			 break;
		case 210:
			 updateConfigInt("bonus2_time_ok", event.parameter);
			 break;
		case 211:
			 updateConfigInt("bonus2_max_mistakes", event.parameter);
			 break;
		case 212:
			 updateConfigInt("bonus2_rand_prob", event.parameter);
			 break;
		case 213:
			 updateConfigInt("bonus3_black_delay", event.parameter);
			 break;
		case 214:
			 updateConfigInt("bonus3_show_delay", event.parameter);
			 break;
		case 215:
			 updateConfigInt("bonus3_black_diff", event.parameter);
			 break;
		case 216:
			 updateConfigInt("bonus3_black_max", event.parameter);
			 break;
		case 217:
			 updateConfigInt("bonus3_black_min", event.parameter);
			 break;
		case 218:
			 updateConfigInt("bonus3_show_diff", event.parameter);
			 break;
		case 219:
			 updateConfigInt("bonus3_show_max", event.parameter);
			 break;
		case 220:
			 updateConfigInt("bonus3_show_min", event.parameter);
			 break;
		case 221:
			 updateConfigInt("bonus3_hide_after_clcick", event.parameter);
		case 222:
			 updateConfigInt("points_D", event.parameter);
			 break;
		case 223:
			 updateConfigInt("points_rjoy_performance", event.parameter);
			 break;
		case 224:
			 updateConfigInt("points_rjoy_maxdist", event.parameter);
			 break;
		case 225:
			 updateConfigInt("points_ljoy_maxdist", event.parameter);
			 break;
		case 226:
			 updateConfigInt("points_mvalue_tb", event.parameter);
			 break;
		case 227:
			 updateConfigFloat("points_mvalue_at_e", event.parameter);
			 break;
		case 228:
			 updateConfigInt("points_mvalue_accvelo_diff", event.parameter);
			 break;
		case 229:
			 updateConfigFloat("points_mvalue_eff_dps", event.parameter);
			 break;
		case 230:
			 updateConfigInt("points_bvalue_remove", event.parameter);
			 break;
		case 231:
			 updateConfigInt("points_b1p_timeok", event.parameter);
			 break;
		case 232:
			 updateConfigInt("points_b2p_value", event.parameter);
			 break;
		case 233:
			 updateConfigInt("points_b2p_newschema", event.parameter);
			 break;
		case 234:
			 updateConfigInt("points_b3p_value", event.parameter);
			 break;
		case 235:
			 updateConfigInt("points_b3p_timeok", event.parameter);
			 break;
		case 236:
			 updateConfigFloat("points_b3p_timeremove", event.parameter);
			 break;
		case 237:
			 updateConfigFloat("points_b1p_remove", event.parameter);
			 break;
		
		default:
			wombatlog("Wrong scenario event nr", 5);

	 }
	
	return 0;
}