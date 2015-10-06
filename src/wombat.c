#include "wombat.h"

int wombatGameStatusReset(wombatGameStatus *status)
{
	int i;
	status->timeleft=getConfigInt("game_time");	
	
	status->x.cat=getConfigInt("taskarea_w")*2/3;
	status->y.cat=getConfigInt("taskarea_h")*2/3;
	status->z.cat=getConfigInt("taskarea_w")/6;	
	
	status->x.m=status->x.cat;
	status->y.m=status->y.cat;
	status->z.m=status->z.cat;
	
	status->x.joy=0;
	status->y.joy=0;
	status->z.joy=0;
	
	status->x.mv=0;
	status->y.mv=0;
	status->z.mv=0;
	
	status->x.catv=0;
	status->y.catv=0;
	status->z.catv=0;
	
	status->x.aimprob = getConfigInt("ai_probx");
	status->y.aimprob = getConfigInt("ai_proby");
	status->z.aimprob = getConfigInt("ai_probz");
	
	status->x.mposmin = getConfigInt("ai_posxmin");
	status->y.mposmin = getConfigInt("ai_posymin");
	status->z.mposmin = getConfigInt("ai_poszmin");

	status->x.mposmax = getConfigInt("ai_posxmin")+getConfigInt("taskarea_w");
	status->y.mposmax = getConfigInt("ai_posymin")+getConfigInt("taskarea_h");
	status->z.mposmax = getConfigInt("ai_poszmax");
	
	status->x.taskarea = getConfigInt("taskarea_w");
	status->y.taskarea = getConfigInt("taskarea_h");
	status->z.taskarea = getConfigInt("ai_poszmax")-getConfigInt("ai_poszmin");
	
	status->x.autoreduce = getConfigInt("circle_r")*getConfigInt("at_maxperformance")/getConfigInt("ai_speed_max");
	status->y.autoreduce = getConfigInt("circle_r")*getConfigInt("at_maxperformance")/getConfigInt("ai_speed_max");
	status->z.autoreduce = getConfigInt("lines_s")*getConfigInt("at_maxperformance")/getConfigInt("ai_speed_max");
	
	status->mode=-1;
		
	status->ifmenu=1;
	
	status->ifbonus=0;
	status->whichbonus=0;
	status->bonustimeleft=0;
	status->bonustasktime=0;
//	status->bonusresult=0;
	status->bonusexittime=SDL_GetTicks();
	status->bonusvalue[0] = getConfigInt("points_value_max")/2;
	status->bonusvalue[1] = getConfigInt("points_value_max")/2;
	status->bonusvalue[2] = getConfigInt("points_value_max")/2;
	status->bonusperformancesuspend;
	
	status->bonus1active=0;
	status->bonus1wfi=0;
	status->bonus1fig1nr=0;
	status->bonus1fig2nr=0;
	
	status->bonus2snr = 0;
	status->bonus2rand = 0;
	
	status->bonus3dlts=0;
	status->bonus3wfi=0;
	status->bonus3digit=0;
	status->bonus3showdelay=getConfigInt("bonus3_show_delay");
	status->bonus3blackdelay=getConfigInt("bonus3_black_delay");

	status->ppoints=0;
	status->ppredicted=0;
	status->pmperformance=00;
	status->pmvalue=0;
	status->pbperformance=0;
	status->pbvalue=0;

	status->pmvaccvelo = getConfigInt("points_mvalue_accvelo_diff");
	status->pmvaccat = getConfigInt("points_mvalue_accvelo_diff")*getConfigInt("va_prob")/getConfigInt("probmax");
	status->pmvate = getConfigInt("points_mvalue_at_e");

	status->pacce = getConfigInt("at_maxperformance");
	status->pveloe = getConfigInt("at_maxperformance");

	for (i=0; i < POINTS_TABLE_SIZE; i++)
	{
		status->ptable[i][0] = status->timeleft;
		status->ptable[i][1] = status->ppoints;
	}

	status->scenario_event_nr=0;
	
	return 0;
}

int mainTaskUpdatePos(wombatGameStatus *status, int timeDelta)
{
	status->x.m += (float)status->x.mv*timeDelta/getConfigInt("ai_speed_d");			// update mouse position
	status->y.m += (float)status->y.mv*timeDelta/getConfigInt("ai_speed_d");
	status->z.m += (float)status->z.mv*timeDelta/getConfigInt("ai_speed_d");

	if (status->x.m > status->x.mposmax)			// prevent mouse from goinf off the taskarea
	{
		status->x.m = status->x.mposmax;
	}
	if (status->x.m < status->x.mposmin)
	{
		status->x.m = status->x.mposmin;
	}
	if (status->y.m > status->y.mposmax)
	{
		status->y.m = status->y.mposmax;
	}
	if (status->y.m < status->y.mposmin)
	{
		status->y.m = status->y.mposmin;
	}
	if (status->z.m > status->z.mposmax)
	{
		status->z.m = status->z.mposmax;
	}
	if (status->z.m < status->z.mposmin)
	{
		status->z.m = status->z.mposmin;
	}

	//printf("Status mode: %i\n", status->mode);
	if (status->mode == -1)								// update cat velocity
	{
		if (abs(status->x.joy) > getConfigInt("joy1_min"))
			status->x.catv = (float)(status->x.joy-getConfigInt("joy1_min"))*getConfigInt("joy1_vmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
		else
			status->x.catv = 0;
			
		if (abs(status->y.joy) > getConfigInt("joy1_min"))
			status->y.catv = (float)(status->y.joy-getConfigInt("joy1_min"))*getConfigInt("joy1_vmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
		else
			status->y.catv = 0;
			
		if (abs(status->z.joy) > getConfigInt("joy2_min"))
			status->z.catv = (float)(status->z.joy-getConfigInt("joy2_min"))*getConfigInt("joy2_vmax")/(getConfigInt("joy2_max")-getConfigInt("joy2_min"));
		else 
			status->z.catv=0;
	}
	
	if (status->ifbonus && status->whichbonus == 1)			// rotate figures in bonus1
	{
		if (status->bonus1active)
		{
			if (abs(status->x.joy) > getConfigInt("joy1_min"))
				status->bonus12rotx += (float)(status->x.joy-getConfigInt("joy1_min"))*timeDelta*getConfigFloat("joy1_rotmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
				
			if (abs(status->y.joy) > getConfigInt("joy1_min"))
				status->bonus12roty += (float)(status->y.joy-getConfigInt("joy1_min"))*timeDelta*getConfigFloat("joy1_rotmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
				
			if (abs(status->z.joy) > getConfigInt("joy2_min"))
				status->bonus12rotz += (float)(status->z.joy-getConfigInt("joy2_min"))*timeDelta*getConfigFloat("joy2_rotmax")/(getConfigInt("joy2_max")-getConfigInt("joy2_min"));
		}
		else 
		{
			if (abs(status->x.joy) > getConfigInt("joy1_min"))
				status->bonus11rotx += (float)(status->x.joy-getConfigInt("joy1_min"))*timeDelta*getConfigFloat("joy1_rotmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
				
			if (abs(status->y.joy) > getConfigInt("joy1_min"))
				status->bonus11roty += (float)(status->y.joy-getConfigInt("joy1_min"))*timeDelta*getConfigFloat("joy1_rotmax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
				
			if (abs(status->z.joy) > getConfigInt("joy2_min"))
				status->bonus11rotz += (float)(status->z.joy-getConfigInt("joy2_min"))*timeDelta*getConfigFloat("joy2_rotmax")/(getConfigInt("joy2_max")-getConfigInt("joy2_min"));
		}
	}
		
		
	
	status->x.cat += (float)status->x.catv*timeDelta/getConfigInt("ai_speed_d");		// update cat position
	status->y.cat += (float)status->y.catv*timeDelta/getConfigInt("ai_speed_d");
	status->z.cat += (float)status->z.catv*timeDelta/getConfigInt("ai_speed_d");
	
	if (status->x.cat > getConfigInt("taskarea_w"))			// prevent cat from going off the window
	{
		status->x.cat = getConfigInt("taskarea_w");
		status->x.catv = 0;
	}
		
	if (status->x.cat < 0)
	{
		status->x.cat = 0;
		status->x.catv=0;
	}
		
	if (status->y.cat > getConfigInt("taskarea_h"))
	{
		status->y.cat = getConfigInt("taskarea_h");
		status->y.catv = 0;
	}
		
	if (status->y.cat < 0)
	{
		status->y.cat = 0;
		status->y.catv = 0;
	}
		
	if (status->z.cat > getConfigInt("ai_poszmax"))
	{
		status->z.cat = getConfigInt("ai_poszmax");
		status->z.catv = 0;
	}
		
	if (status->z.cat < getConfigInt("ai_poszmin"))
	{
		status->z.cat = getConfigInt("ai_poszmin");
		status->z.catv = 0;
	}
	
	return 0;
}

int mainTaskAI(wombatMainPart *element)
{
	int number;
	char text[40];

	number = rand()%getConfigInt("probmax");			// changing Velocity
	sprintf(text, "AI: random nr: %i", number);
	wombatlog(text, 1);
	if (number < element->aimprob || element->mv < getConfigInt("ai_force_small_speed"))
	{
		number = rand()%element->taskarea;		// probability of going right increases when being left
		sprintf(text, "AI: Change speed: %i ~ %f", number, element->m);
		wombatlog(text, 1);
		if (number>element->m)
			element->mv += rand()%getConfigInt("ai_speed_m");
		else 
			element->mv -= rand()%getConfigInt("ai_speed_m");
	}
	
	if (abs(element->mv) < getConfigInt("ai_force_small_speed"))
	{
		wombatlog("AI: Force small speed", 2);
		element->mv *= getConfigFloat("ai_force_speed_value");
	}
				
	if (element->mv > getConfigInt("ai_speed_max"))		// prevention from exeedeing max speed
		element->mv = getConfigInt("ai_speed_max");
	if (element->mv < -getConfigInt("ai_speed_max"))
		element->mv = -getConfigInt("ai_speed_max");				

	
	if (element->m < element->mposmin)		// prevention from losing task area
	{
		element->m = element->mposmin;
		element->mv = 0;
	}
	if (element->m > element->mposmax)
	{
		element->m = element->mposmax;
		element->mv = 0;
	}	
	
	if (element->m <= element->mposmin+getConfigInt("ai_posborder"))	// decresing speed in border area
	{
		wombatlog("AI: mouse reached border area", 1);
		element->mv += getConfigInt("ai_borderspeedadd");
	}
	
	if (element->m >= element->mposmax-getConfigInt("ai_posborder"))
	{
		wombatlog("AI: mouse reached border area", 1);
		element->mv -= getConfigInt("ai_borderspeedadd");
	}	

	return 0;	
}

int mainTaskAuto(wombatMainPart *element, int performance, int timeDelta)
{	
	element->catv = (element->m-element->cat)*performance*timeDelta/element->autoreduce/getConfigInt("at_updateinterval");
	if (abs(element->catv) < 1)		// this condition makes failure more efficient
		element->catv=0;
	
	return 0;
}

int AutotrackRun(wombatGameStatus *status, wombatTimer *timer)
{
	if (AutoConditions(status))
	{
		wombatlog("AT: Running", 1);
		timer->Autotrack.timer = SDL_AddTimer(getConfigInt("at_updateinterval"), AutoTimerFunction, NULL);
		timer->Autotrack.run = 1;
		timer->AutotrackFailure.timer = SDL_AddTimer(getConfigInt("at_failureinterval"), AutoFailureFunction, NULL);
		timer->AutotrackFailure.run = 1;
		SDL_RemoveTimer(timer->VeloAcce.timer);
		timer->VeloAcce.run=0;
		SDL_RemoveTimer(timer->AccelUpdate.timer);
		timer->AccelUpdate.run=0;
		status->mode = getConfigInt("at_maxperformance");
		status->at_timelast = SDL_GetTicks();
		status->x.catv = 0;
		status->y.catv = 0;
		status->z.catv = 0;
	}
	
	return 0;
}

int AutotrackStop(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("AT: Stopped", 1);
	if (status->mode > status->pmvate)		// points for finding autotrack failure
	{
		status->pmvate += (status->pmvate-status->mode)/getConfigFloat("points_mvalue_at_e_add");
	}
	status->mode = -1;
	if (getConfigInt("va_start"))
	{
		VeloAcceChange(status, timer);
	}
	status->x.catv=0;
	status->y.catv=0;
	status->z.catv=0;
	SDL_RemoveTimer(timer->Autotrack.timer);
	timer->Autotrack.run=0;
	SDL_RemoveTimer(timer->AutotrackFailure.timer);
	timer->AutotrackFailure.run=0;
	timer->VeloAcce.timer = SDL_AddTimer(getConfigInt("va_timerinterval"), VeloAcceFunction, NULL);
	timer->VeloAcce.run = 1;
	if (status->failure)	
	{
		timer->AutotrackRepair.timer = SDL_AddTimer(getConfigInt("at_repairinterval"), AutoRepairFunction, NULL);
		timer->AutotrackRepair.run=1;
	}
	
	return 0;
}

int AutoConditions(wombatGameStatus *status)
{
	wombatlog("Checking autotrack conditions", 1);
	if (status->failure)
		return 0;
	if (abs(status->x.cat-status->x.m) > getConfigInt("circle_r"))
		return 0;
	if (abs(status->y.cat-status->y.m) > getConfigInt("circle_r"))
		return 0;
	if (abs(status->z.cat-status->z.m) > getConfigInt("lines_s"))
		return 0;

	wombatlog("Conditions OK", 1);
		return 1;
}		

int AutoFail(wombatGameStatus *status, wombatTimer *timer)
{
	if (status->mode < 1)
	{
		return -1;
	}
	if (rand()%getConfigInt("at_fail_randmax") > status->mode)
	{
		wombatlog("Autotrack failure", 2);
		status->failure = 1;
		if (status->mode < status->pmvate)
			status->pmvate = status->mode;
		if(status->mode > getConfigInt("at_fail_remove"))
			status->mode -= getConfigInt("at_fail_remove");
	}
	return 0;
}
	
int AutoRepair(wombatGameStatus *status, wombatTimer *timer)
{
	if (!status->failure)
	{
		return -1;
	}
	if (rand()%getConfigInt("probmax") < getConfigInt("at_repair_prob"))
	{
		wombatlog("Autotrack repair", 2);
		status->failure=0;		
		SDL_RemoveTimer(timer->AutotrackRepair.timer);
		timer->AutotrackRepair.run=0;
	}
	return 0;
}

int VeloAcce(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("Vello/accel change function", 1);
	if (rand()%getConfigInt("probmax") < getConfigInt("va_prob"))
	{
		VeloAcceChange(status, timer);
	}

	return 0;
}

int VeloAcceChange(wombatGameStatus *status, wombatTimer *timer)
{	
	if (status->mode > 0)
	{
		return -1;
	}
	if(status->mode == 0)
	{
		wombatlog("Change to velocity mode", 2);
		status->mode = -1;
		SDL_RemoveTimer(timer->AccelUpdate.timer);
		timer->AccelUpdate.run=0;
	}
	else
	{
		if (status->mode == -1)
		{
			wombatlog("Change to acceleration mode", 2);
			status->mode = 0;
			timer->AccelUpdate.timer = SDL_AddTimer(getConfigInt("va_updateaccelinterval"), AccelUpdateFunction, NULL);
			timer->AccelUpdate.run = 1;
		}
	}


	return 0;
}

int AccelUpdate(wombatGameStatus *status)
{	
	if (status->mode == 0)							// update cat acceleration
	{
		wombatlog("Updating cat acceleration", 1);
		if (abs(status->x.joy) > getConfigInt("joy1_min"))
		{
			status->x.catv += (float)(status->x.joy-getConfigInt("joy1_min"))*getConfigInt("joy1_amax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"))/getConfigInt("joy1_accel_d");
		}
		
		if (abs(status->y.joy) > getConfigInt("joy1_min"))	
			status->y.catv += (float)(status->y.joy-getConfigInt("joy1_min"))*getConfigInt("joy1_amax")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"))/getConfigInt("joy1_accel_d");
			
		if (abs(status->z.joy) > getConfigInt("joy2_min"))
			status->z.catv += (float)(status->z.joy-getConfigInt("joy2_min"))*getConfigInt("joy2_amax")/(getConfigInt("joy2_max")-getConfigInt("joy2_min"))/getConfigInt("joy2_accel_d");
	}

	return 0;
}

int UpdateStatusParameter(int nr, int value, wombatGameStatus *status, wombatTimer *timer)
{
	switch (nr)
	{
		case 110:
			status->x.m = value;
			break;
		case 111:
			status->y.m = value;
			break;
		case 112:
			status->z.m = value;
			break;
		case 113:
			status->x.mv = value;
			break;
		case 114:
			status->y.mv = value;
			break;
		case 115:
			status->z.mv = value;
			break;
		case 116:
			status->x.aimprob = value;
			break;
		case 117:
			status->y.aimprob = value;
			break;
		case 118:
			status->z.aimprob = value;
			break;
		case 119:
			status->x.mposmin = value;
			break;
		case 120:
			status->x.mposmax = value;
			break;
		case 121:
			status->y.mposmin = value;
			break;
		case 122:
			status->y.mposmax = value;
			break;
		case 123:
			status->z.mposmin = value;
			break;
		case 124:
			status->z.mposmax = value;
			break;
		case 131:
			status->bonus2snr = value;
			break;
	}

	return 0;
}

int wombatSave(wombatGameStatus *status, wombatTimer *timer, char *filename)
{
	FILE *file2save;
	char version[VERSION_SIZE];

	strncpy(version, PACKAGE_STRING, VERSION_SIZE);

	file2save = fopen(filename, "wb");
	if (file2save == NULL)
	{
		return 1;
	}

	if (fwrite (version, sizeof(char), VERSION_SIZE, file2save) != VERSION_SIZE)
	{
		return 1;
	}

	if (fwrite (status, sizeof(wombatGameStatus), 1, file2save) != 1)
	{
		return 1;
	}

	if (fwrite (timer, sizeof(wombatTimer), 1, file2save) != 1)
	{
		return 1;
	}

	fclose(file2save);

	return 0;
}
	
int wombatLoad(wombatGameStatus *status, wombatTimer *timer, char *filename)
{
	FILE *file2load;
	char version[VERSION_SIZE];

	file2load = fopen (filename, "rb");
	if (file2load == NULL)
	{
		return 1;
	}

	if (fread(version, sizeof(char), VERSION_SIZE, file2load) != VERSION_SIZE*sizeof(char))
	{
		return 1;
	}

	if (strncmp(version, PACKAGE_STRING, VERSION_SIZE) != 0)		// version not compatibile
	{
		return 1;
	}

	if (fread(status, sizeof(wombatGameStatus), 1, file2load) != 1)
	{
		wombatGameStatusReset (status);
		return 1;
	}

	if (fread(timer, sizeof(wombatTimer), 1, file2load) != 1)
	{
		wombatGameStatusReset (status);
		return 1;
	}

	if (status->scenario_name[0] != '\0')		// if saved game has a scenario
	{
		if (scenario_check_name(status->scenario_name) != 0)
		{
			wombatGameStatusReset (status);
			wombatlog("No scenario found!", 10);
			return 1;
		}
		//setDefaultConfig();
		loadConfig();
		Bonus1RemoveFigures();
		Bonus1ReadFigures(status->scenario_name);
	}

	if (status->bonus1fig1nr >= getConfigInt("bonus1_fig_nr") || status->bonus1fig2nr > getConfigInt("bonus1_fig_nr"))
	{
		wombatGameStatusReset (status);
		return 1;
	}

	fclose(file2load);	// loaded successfully
	return 0;
}


int wombatTimersStart(wombatTimer *timer)
{
	timer->AI.run=0;
	timer->VeloAcce.run=0;
	timer->AccelUpdate.run=0;
	timer->Autotrack.run=0;
	timer->AutotrackFailure.run=0;
	timer->AutotrackRepair.run=0;
	timer->Bonus.run=0;
	timer->Bonus31.run=0;
	timer->Bonus32.run=0;

	return 0;
}

int wombatTimersSuspend(wombatTimer *timer)
{
	wombatTimerSuspend(&(timer->AI));
//	wombatlog("AI", 5);
	wombatTimerSuspend(&(timer->VeloAcce));
//	wombatlog("VeloAcce", 5);
	wombatTimerSuspend(&(timer->AccelUpdate));
//	wombatlog("AccelUpdate", 5);
	wombatTimerSuspend(&(timer->Autotrack));
//	wombatlog("Autotrack", 5);
	wombatTimerSuspend(&(timer->AutotrackFailure));
//	wombatlog("Failure", 5);
	wombatTimerSuspend(&(timer->AutotrackRepair));
	//wombatlog("Repair", 5);
	wombatTimerSuspend(&(timer->Bonus));
//	wombatlog("Bonus", 5);
	wombatTimerSuspend(&(timer->Bonus31));
//	wombatlog("B31", 5);
	wombatTimerSuspend(&(timer->Bonus32));
//	wombatlog("B32", 5);

	return 0;
}

int wombatTimersResume(wombatTimer *timer, wombatGameStatus *status)
{
	wombatTimerResume(&(timer->AI), getConfigInt("ai_timerinterval"), AITimerFunction);
//	wombatlog("AI", 5);
	wombatTimerResume(&(timer->VeloAcce), getConfigInt("va_timerinterval"), VeloAcceFunction);
//	wombatlog("VeloAcce", 5);
	wombatTimerResume(&(timer->AccelUpdate), getConfigInt("va_updateaccelinterval"), AccelUpdateFunction);
//	wombatlog("AccelUpdate", 5);
	wombatTimerResume(&(timer->Autotrack), getConfigInt("at_updateinterval"), AutoTimerFunction);
//	wombatlog("Autotrack", 5);
	wombatTimerResume(&(timer->AutotrackFailure), getConfigInt("at_failureinterval"), AutoFailureFunction);
//	wombatlog("Failure", 5);
	wombatTimerResume(&(timer->AutotrackRepair), getConfigInt("at_repairinterval"), AutoRepairFunction);
//	wombatlog("Repair", 5);
	wombatTimerResume(&(timer->Bonus), getConfigInt("bonus_removeinterval"), BonusFunction);
//	wombatlog("Bonus", 5);
	wombatTimerResume(&(timer->Bonus31), status->bonus3blackdelay, Bonus31Function);
//	wombatlog("B31", 5);
	wombatTimerResume(&(timer->Bonus32), status->bonus3showdelay, Bonus32Function);
//	wombatlog("B32", 5);

	return 0;
}

int wombatTimerSuspend(wombatTimerElement *id)
{
	if (id->run)
	{
		SDL_RemoveTimer(id->timer);
	}

	return 0;
}

int wombatTimerResume(wombatTimerElement *id, Uint32 interval, SDL_NewTimerCallback callback)
{
	if (id->run)
	{
		id->timer = SDL_AddTimer(interval, callback, NULL);
	}

	return 0;
}
	