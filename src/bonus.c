#include "bonus.h"

int BonusRemoveTime(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("Bonus: Time left is smaller", 1);
	status->bonustimeleft -= getConfigInt("bonus_remove");
	status->bonusexittime = SDL_GetTicks();
	if (status->whichbonus == 1 && status->bonus1wfi)	// counting points in bonus 1
	{
		if (SDL_GetTicks()-status->bonusstarttime + status->bonustasktime > getConfigInt("points_b1p_timeok"))
		{
			status->pbperformance = getConfigInt("points_performance_max") - 
				(SDL_GetTicks()-status->bonusstarttime + status->bonustasktime - getConfigInt("points_b1p_timeok")) *
				getConfigFloat("points_b1p_remove")/1000;
		}
		if (status->pbperformance < 0)
			status->pbperformance = 0;
	}
	if (status->whichbonus == 3 && status->bonus3wfi)	// counting points in bonus 3   AAAA cos tu jest zle!
	{
		if (SDL_GetTicks()-status->bonusstarttime + status->bonustasktime > getConfigInt("points_b3p_timeok"))
		{
			pointsBonusPerformanceUpdate(status, (int)-getConfigFloat("points_b3p_timeremove"));
		}
		if (status->pbperformance < 0)
			status->pbperformance = 0;
	}
	if (status->bonustimeleft <= 0)  // time is up
	{
		wombatlog("Bonus: No time left", 2);
		status->pbperformance = 0;
		status->bonustimeleft = 0;
		status->whichbonus = 0;
//		status->bonusresult = 0;
		SDL_RemoveTimer(timer->Bonus.timer);
		timer->Bonus.run = 0;
	}
	
	return 0;
}

int Bonus1start(wombatGameStatus *status)
{
	wombatlog("Bonus1: start", 2);
//	status->bonusresult = 0;
	status->bonus1mode = rand()%3;	
	status->bonus1fig1nr=rand()%getConfigInt("bonus1_fig_nr");
	
	if (status->bonus1mode == 2)
		status->bonus1mode += rand()%2;

	if ( status->bonus1mode == 2 || status->bonus1mode == 3) // if different
	{
		status->bonus1fig2nr=rand()%getConfigInt("bonus1_fig_nr");
		while(status->bonus1fig1nr == status->bonus1fig2nr)
		{
			status->bonus1fig2nr=rand()%getConfigInt("bonus1_fig_nr");	
		}	
	}
	else 
		status->bonus1fig2nr = status->bonus1fig1nr;
	
	status->bonus11rotx=(float)((rand()%200));
	status->bonus11roty=(float)((rand()%200));
	status->bonus11rotz=(float)((rand()%200));
	status->bonus12rotx=(float)((rand()%200));
	status->bonus12roty=(float)((rand()%200));
	status->bonus12rotz=(float)((rand()%200));
	
	status->pbperformance = getConfigInt("points_performance_max");
	
	status->bonustasktime = 0;
	status->bonusstarttime = SDL_GetTicks();
	
	status->bonus1wfi = 1;
	
	return 0;
}

int Bonus1Input(wombatGameStatus *status, int nr)
{
	if (!status->bonus1wfi)
		return 0;

	if (status->bonus1mode == nr || (status->bonus1mode == 3 && nr == 2))
	{
//		status->bonusresult = 1;
		Bonus1start(status);
	}
	else 
	{
		status->pbperformance = 0;
//		status->bonusresult = -1;
		status->bonus1wfi=0;
	}
	
	return 0;
}



int Bonus2start(wombatGameStatus *status)
{
	wombatlog("Bonus2: start", 2);
	int i=0, j=1, k, ile[4];
	char filename[WOMBAT_FILENAME_SIZE];
	FILE *bonus2file;	
	unsigned short int **bonus2schema;
	unsigned short int **bonus2rectangles[4];		// array that shows which cells are empty

	bonus2schema = (unsigned short int **)malloc(sizeof(unsigned short int *));

	if (status->scenario_name[0] != '\0')
	{
		wombatlog("Trying scenario schema file", 1);
		strncpy(filename, SCENARIO_PATH, WOMBAT_FILENAME_SIZE);
		strncat(filename, "bonus2schemas", WOMBAT_FILENAME_SIZE-strlen(filename));
		bonus2file = fopen(filename, "r");
		if (bonus2file == NULL)
		{
			wombatlog("Can't open scenario file to load schemas", 5);
		}
		else
		{
			while (j)
			{
				bonus2schema = (unsigned short int **)realloc(bonus2schema, (i+1)*sizeof(unsigned short int *));
				bonus2schema[i] = (unsigned short int *)malloc(getConfigInt("bonus2_numbers")*sizeof(unsigned short int));
				for (k=0; k<getConfigInt("bonus2_numbers"); k++)
				{
					if (fscanf(bonus2file, "%i,", (int *)&bonus2schema[i][k]) != 1)
					{
						wombatlog("Error in bonus2 schema file", 5);
						j=0;
						break;
					}
				}
				i++;
			}
			fclose(bonus2file);
		}
	}
	
	if (i < 2)
	{
		wombatlog("Trying user schema file", 1);
		i=0;
		j=1;

		configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "bonus2schemas", PACKAGE);
		bonus2file = fopen(filename, "r");
		if (bonus2file == NULL)
		{
			wombatlog("Can't open user file to load schemas", 5);
		}
		else
		{
			while (j)
			{
				bonus2schema = (unsigned short int **)realloc(bonus2schema, (i+1)*sizeof(unsigned short int *));
				bonus2schema[i] = (unsigned short int *)malloc(getConfigInt("bonus2_numbers")*sizeof(unsigned short int));
				for (k=0; k<getConfigInt("bonus2_numbers"); k++)
					{
					if (fscanf(bonus2file, "%i,", (int *)&bonus2schema[i][k]) != 1)
					{
						wombatlog("Error in bonus2 schema file", 5);
						j=0;
						break;
					}
				}
				i++;
			}
			fclose(bonus2file);
		}
	}
	
#ifdef DATADIR

	if (i < 2)
	{
		wombatlog("Trying system schema file", 1);
		i=0;
		j=1;
		strncpy(filename, DATADIR, WOMBAT_FILENAME_SIZE);
		strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));
		strncat(filename, PACKAGE, WOMBAT_FILENAME_SIZE-strlen(filename));
		strncat(filename, "/bonus2schemas", WOMBAT_FILENAME_SIZE-strlen(filename));
		bonus2file = fopen(filename, "r");
		if (bonus2file == NULL)
		{
			wombatlog("Can't open global file to load schemas", 5);
			return 1;
		}	

		while (j)
		{
			bonus2schema = (unsigned short int **)realloc(bonus2schema, (i+1)*sizeof(unsigned short int *));
			bonus2schema[i] = (unsigned short int *)malloc(getConfigInt("bonus2_numbers")*sizeof(unsigned short int));
			for (k=0; k<getConfigInt("bonus2_numbers"); k++)
			{
				if (fscanf(bonus2file, "%i,", (int *)&bonus2schema[i][k]) != 1)
				{
					wombatlog("Error in bonus2 schema file", 5);
					j=0;
					break;
				}
			}
			i++;
		}
		fclose(bonus2file);
	}
#endif	
	updateConfigInt("bonus2_schemas", i-1);

	for (i=0; i<4; i++)
	{
		bonus2rectangles[i] = (unsigned short int **)malloc(getConfigInt("bonus2_cols")*sizeof(unsigned short int *));
		for (j=0; j<getConfigInt("bonus2_cols"); j++)
		{
			bonus2rectangles[i][j] = (unsigned short int *)malloc(getConfigInt("bonus2_rows")*sizeof(unsigned short int));
		}
	}
	
//	status->bonusresult = 0;	
	
	for (i=0; i<4; i++)
	{
		for (j=0; j<getConfigInt("bonus2_cols"); j++)
		{
			for (k=0; k<getConfigInt("bonus2_rows"); k++)
			{
				bonus2rectangles[i][j][k] = 0;
			}
		}
	}

	for (j=0; j<4; j++)
		ile[j]=0;

	if (rand()%getConfigInt("probmax") < getConfigInt("bonus2_rand_prob"))			// check if use random schema
	{
		status->bonus2rand=1;
	}
	else
	{
		status->bonus2rand=0;
	}
	
	for (i=0; i<32; i++)
	{			
		if (status->bonus2rand || getConfigInt("bonus2_schemas") == 0)
		{
			j = rand()%4;
			while (ile[j] >= getConfigInt("bonus2_numbers")/4)
				j = rand()%4;
				
			ile[j]++;
			status->bonus2table[i][0] = j+1;
		}
		else
			status->bonus2table[i][0] = bonus2schema[status->bonus2snr][i];
		
		j = rand()%getConfigInt("bonus2_cols");
		k = rand()%getConfigInt("bonus2_rows");
		while(bonus2rectangles[status->bonus2table[i][0]-1][j][k])
		{
			j = rand()%getConfigInt("bonus2_cols");
			k = rand()%getConfigInt("bonus2_rows");	
		}
		bonus2rectangles[status->bonus2table[i][0]-1][j][k] = 1;
		status->bonus2table[i][1] = j;
		status->bonus2table[i][2] = k; 
	}

//	for (i=0; i<getConfigInt("bonus2_numbers"); i++)
//		free(bonus2schema[i]);
	free(bonus2schema);

	for (i=0; i<4; i++)
	{
		for (j=0; j<getConfigInt("bonus2_cols"); j++)
		{
			/*for (k=0; k<getConfigInt("bonus2_rows"); k++)
			{
				free(bonus2rectangles[i][j][k]);*/
			free(bonus2rectangles[i][j]);
		}
		free(bonus2rectangles[i]);
	}
	
	status->bonus2whichnr = 0;
	status->bonustasktime = 0;
	status->bonus2mistakes = 0;
	status->bonusstarttime = SDL_GetTicks();
	
	return 0;
}

int Bonus2Input(wombatGameStatus *status, int rectangle)
{
	if (rectangle == status->bonus2table[status->bonus2whichnr][0])
	{
		pointsBonusPerformanceUpdate(status, getConfigInt("points_b2p_value"));
		status->bonus2whichnr++;
	}
	else 
	{
		pointsBonusPerformanceUpdate(status, -getConfigInt("points_b2p_value"));
		status->bonus2mistakes++;
	}
	
	if (status->bonus2whichnr >= getConfigInt("bonus2_numbers"))		// if finished
	{
		status->bonustasktime += SDL_GetTicks()-status->bonusstarttime;
		if (status->bonus2rand == 0)						// if last schema was random, next one should not	
		{
			//printf("bonus2_time_ok: %i\nbonustasktime: %i\nbonus2snr: %i\n", Bonus2TimeOK, status->bonustasktime, status->bonus2snr);
			if (status->bonustasktime < getConfigInt("bonus2_time_ok") && status->bonus2mistakes <= getConfigInt("bonus2_max_mistakes"))		// if try next schema
			{
				wombatlog("Bonus2: using next schema", 2);
				pointsAdd(status, getConfigInt("points_b2p_newschema"), 1);
				status->bonus2snr++;
			}	
		}
							

	
		if (status->bonus2snr > getConfigInt("bonus2_schemas")-1)
			status->bonus2snr=0;
		
		Bonus2start(status);
	}
	
	return 0;
}

int Bonus3start(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("Bonus3: start", 2);
	status->bonus3wfi = 0;
	status->bonus3dlts = 3;
	status->bonus3d[0] = rand()%8+1;
	status->bonus3d[1] = rand()%8+1;
	status->bonus3d[2] = rand()%8+1;
	
	Bonus3(status, timer);

	return 0;	
}

int Bonus3(wombatGameStatus *status, wombatTimer *timer)
{
	if (status->bonus3dlts)
	{
		wombatlog("Bonus3: show digit", 1);
		status->bonus3digit = 0;
		timer->Bonus31.timer = SDL_AddTimer(status->bonus3blackdelay, Bonus31Function, NULL);	
		timer->Bonus31.run = 1;
		status->bonustasktime = 0;
		status->bonusstarttime = SDL_GetTicks();
	}
	else	
	{
		wombatlog("Bonus3: hide last digit", 1);
		status->bonus3digit = 0;
	}	

	return 0;	
}

int Bonus31(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("Bonus3: showing digit", 2);
	status->bonus3dlts--;
	status->bonus3digit = status->bonus3d[status->bonus3dlts];
	timer->Bonus32.timer = SDL_AddTimer(status->bonus3showdelay, Bonus32Function, NULL);
	timer->Bonus32.run = 1;
	SDL_RemoveTimer(timer->Bonus31.timer);
	timer->Bonus31.run = 0;
	status->bonustasktime = 0;
	status->bonusstarttime = SDL_GetTicks();
	if (status->bonus3dlts == 0)
	{
		status->bonus3wfi=1;
	}
	
	
	return 0;
}

int Bonus32(wombatGameStatus *status, wombatTimer *timer)
{
	wombatlog("Bonus3: waiting...", 2);
	SDL_RemoveTimer(timer->Bonus32.timer);
	timer->Bonus32.run = 0;
	timer->Bonus32.timer = NULL;
	Bonus3(status, timer);
	
	return 0;
}

int Bonus3Input(wombatGameStatus *status, wombatTimer *timer, int nr)
{
	if (!status->bonus3wfi)
		return 0;

//	SDL_RemoveTimer(timer->Bonus32);
	wombatlog("Bonus3: Input", 2);
	
	status->bonus3wfi=0;

	if (getConfigInt("bonus3_hide_after_click"))
	{
		status->bonus3digit=0;
	}
	
	if (nr == status->bonus3d[2])
	{
		pointsBonusPerformanceUpdate(status, getConfigInt("points_b3p_value"));
		Bonus3ChangeDelay (status, -1, -1);
	}
	else 
	{
		pointsBonusPerformanceUpdate(status, -getConfigInt("points_b3p_value"));
		Bonus3ChangeDelay(status, 1, 1);
	}
		
	status->bonus3d[2] = status->bonus3d[1];
	status->bonus3d[1] = status->bonus3d[0];
	status->bonus3d[0] = rand()%8+1;

	status->bonus3dlts++;

	status->bonustasktime += SDL_GetTicks()-status->bonusstarttime;

	if (timer->Bonus32.timer == NULL)
		Bonus3(status, timer);
	
	return 0;
}

int Bonus3ChangeDelay(wombatGameStatus *status, int showvalue, int blackvalue)
{
	int diff = getConfigInt("bonus3_show_diff");
	int max = getConfigInt("bonus3_show_max");
	int min = getConfigInt("bonus3_show_min");

	status->bonus3showdelay += showvalue*diff;
	
	if (status->bonus3showdelay > max)
	{
		status->bonus3showdelay = max;
	}
	if (status->bonus3showdelay < min)
	{
		status->bonus3showdelay = min;
	}

	diff = getConfigInt("bonus3_black_diff");
	max = getConfigInt("bonus3_black_max");
	min = getConfigInt("bonus3_black_min");

	status->bonus3blackdelay += blackvalue*diff;
	
	if (status->bonus3blackdelay > max)
	{
		status->bonus3blackdelay = max;
	}
	if (status->bonus3blackdelay < min)
	{
		status->bonus3blackdelay = min;
	}
	

	return 0;
}
