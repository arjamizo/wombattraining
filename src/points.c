/***************************************************************************
 *            points.c
 *
 *  Fri Nov 20 00:26:48 2009
 *  Copyright  2009  bearh
 *  <bearh@<host>>
 ****************************************************************************/

#include "points.h"

static points_to_log points_memory;

int pointsCount(wombatGameStatus *status, int timeDelta)
{
	float lastpoints_track, lastpoints_bonus;
	
	pointsMaintaskPerformanceUpdate(status, timeDelta);
	pointsMaintaskValueUpdate (status);

	lastpoints_track = (float)(status->pmperformance*status->pmvalue)*timeDelta/getConfigInt("points_D")/10000/24;
	lastpoints_bonus = (float)(status->pbperformance*status->pbvalue)*timeDelta/getConfigInt("points_D")/10000/24;
	
	status->ppoints += lastpoints_track+lastpoints_bonus;
		

	pointsPredict (status);

	points_memory.prediction = status->ppredicted;
	points_memory.all = status->ppoints;
	points_memory.from_lasttime_all += lastpoints_track+lastpoints_bonus;
	points_memory.autotrack += lastpoints_track;
	points_memory.autotrack_all += lastpoints_track;
	if (status->ifbonus)
	{
		switch (status->whichbonus)
		{
			case 1:
				points_memory.bonus1 += lastpoints_bonus;
				points_memory.bonus1_all += lastpoints_bonus;
				break;
			case 2:
				points_memory.bonus2 += lastpoints_bonus;
				points_memory.bonus2_all += lastpoints_bonus;
				break;
			case 3:
				points_memory.bonus3 += lastpoints_bonus;
				points_memory.bonus3_all += lastpoints_bonus;
		}
	}

	return 0;
}

int pointsPredict(wombatGameStatus *status)
{
	int i;

	if (status->timeleft < status->ptable[0][0] - getConfigInt("points_prediction_time")/POINTS_TABLE_SIZE)
	{
		for (i=POINTS_TABLE_SIZE-1; i>0; i--)
		{
			status->ptable[i][0] = status->ptable[i-1][0];
			status->ptable[i][1] = status->ptable[i-1][1];
		}

		status->ptable[0][0] = status->timeleft;
		status->ptable[0][1] = status->ppoints;

		status->ppredicted = status->timeleft * (status->ptable[0][1] - status->ptable[POINTS_TABLE_SIZE-1][1]) /	// how many points counted during points_prediction_time
			(status->ptable[POINTS_TABLE_SIZE-1][0] - status->ptable[0][0]) + status->ppoints;
	}

	return 0;
}

int pointsMaintaskPerformanceUpdate(wombatGameStatus *status, int timeDelta)
{
	float distance;
	int maxperformance;
	status->pmperformance = 0;

	// right joystick
	distance = sqrt(pow(status->x.cat-status->x.m, 2) + pow(status->y.cat-status->y.m, 2));
	maxperformance = getConfigInt("points_rjoy_performance");
	
	status->pmperformance += maxperformance;
	if (distance > getConfigInt("circle_r") &&
	    distance <= getConfigInt("circle_r") + getConfigInt("points_rjoy_maxdist"))
	{
		status->pmperformance -= (distance - getConfigInt("circle_r")) / 
			getConfigInt("points_rjoy_maxdist") * maxperformance;
	}
	if (distance > getConfigInt("circle_r") + getConfigInt("points_rjoy_maxdist"))
	{
		status->pmperformance -= maxperformance;
	}

	if (distance > getConfigInt("circle_r"))		// efficiency indicators
	{
		if (status->mode == -1)
		{
			status->pveloe -= getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
		if (status->mode == 0)
		{
			status->pacce -= getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
	}
	else
	{
		if (status->mode == -1)
		{
			status->pveloe += getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
		if (status->mode == 0)
		{
			status->pacce += getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
	}
		

	// left joystick
	distance = abs(status->z.cat - status->z.m);
	maxperformance = getConfigInt("points_performance_max") - getConfigInt("points_rjoy_performance");
	
	status->pmperformance += maxperformance;
	if (distance > getConfigInt("lines_s") &&
	    distance <= getConfigInt("lines_s") + getConfigInt("points_ljoy_maxdist"))
	{
		status->pmperformance -= (distance - getConfigInt("lines_s")) / 
			getConfigInt("points_ljoy_maxdist") * maxperformance;
	}
	if (distance > getConfigInt("lines_s") + getConfigInt("points_ljoy_maxdist"))
	{
		status->pmperformance -= maxperformance;
	}

	if (distance > getConfigInt("lines_s"))		// efficiency indicators
	{
		if (status->mode == -1)
		{
			status->pveloe -= getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
		if (status->mode == 0)
		{
			status->pacce -= getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
	}
	else
	{
		if (status->mode == -1)
		{
			status->pveloe += getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
		if (status->mode == 0)
		{
			status->pacce += getConfigFloat("points_mvalue_eff_dps")*timeDelta/1000;
		}
	}

	if (status->pveloe > getConfigInt("at_maxperformance"))
		status->pveloe = getConfigInt("at_maxperformance");
	if (status->pveloe < 0)
		status->pveloe = 0;
	if (status->pacce > getConfigInt("at_maxperformance"))
		status->pacce = getConfigInt("at_maxperformance");
	if (status->pacce < 0)
		status->pacce = 0;


	return 0;
}


int pointsMaintaskValueUpdate(wombatGameStatus *status)
{
	int timenow;

	timenow = SDL_GetTicks();

	status->pmvalue = getConfigInt("points_value_max");


	status->pmvaccvelo = getConfigInt("points_mvalue_accvelo_diff") - 
		(status->pacce - status->pveloe)*getConfigFloat("points_mvalue_eff_diff_m");

	if (status->mode == -1)
	{
		status->pmvalue -= status->pmvaccvelo;
	}

	if (status->mode > 0)
	{
		status->pmvalue -= status->pmvaccat;
	}

	if (timenow - status->bonusexittime > getConfigInt("points_mvalue_tb"))
	{
		status->pmvalue -= getConfigFloat("points_mvalue_tb_remove")*
			(timenow-status->bonusexittime-getConfigInt("points_mvalue_tb"))/1000;
	}

	status->pmvalue *= status->pmvate/100;

	if (status->pmvalue < 0)
	{
		status->pmvalue = 0;
	}
	

	return 0;
}

int pointsBonusValueUpdate(wombatGameStatus *status, int nr)
{
	int i;
	
	nr--; // nr takes values 1-3 and here goes 0-2

	status->pbvalue = status->bonusvalue[nr];
	
	if (status->bonusvalue[nr] <= 0)
	{
		return 0;
	}
	
	for (i=0; i<3; i++)
	{
		if (i == nr)
		{
			status->bonusvalue[i] -= getConfigInt("points_bvalue_remove");
			if (status->bonusvalue[i] < 0)
			{
				status->bonusvalue[i] = 0;
			}
		}
		else
		{
			status->bonusvalue[i] += getConfigInt("points_bvalue_remove")/2;
			if (status->bonusvalue[i] > getConfigInt("points_value_max"))
			{
				status->bonusvalue[i] = getConfigInt("points_value_max");
			}
		}
	}

	return 0;
}

int pointsBonusPerformanceUpdate(wombatGameStatus *status, int value)
{
	status->pbperformance += value;

	if (status->pbperformance > getConfigInt("points_performance_max"))
	{
		status->pbperformance = getConfigInt("points_performance_max");
	}
	if (status->pbperformance < 0)
	{
		status->pbperformance = 0;
	}

	return 0;
}

int pointsAdd(wombatGameStatus *status, int value, int why)
{
	status->ppoints += value;

	switch (why)
	{
		case 1:	// found bonus2 schema
			points_memory.bonus2howmanysolved++;
			points_memory.bonus2solved+=value;
			points_memory.bonus2howmanysolved_all++;
			points_memory.bonus2solved_all+=value;
			break;
	}

	return 0;
}

int pointsLog(wombatGameStatus *status)
{
	FILE *logfile;

	if (getConfigString("points_log_file")[0] == '!')	// check if logging
	{
		return 0;
	}

	if (points_memory.lasttime == 0)		// set variable at the begginning and write heading to file
	{
		points_memory.lasttime = status->timeleft;
		
		logfile = fopen(getConfigString("points_log_file"), "a");
		if (logfile == NULL)
		{
			wombatlog("Can't open file to log points", 5);
			return -1;
		}

		fprintf(logfile, "----------+----------+----------+----------+--------------+--------+----------+-----------+-----------\n");
		fprintf(logfile, "Time left:|Tracking: |Bonus1:   |Bonus2:   |Solved points:|Schemas:|Bonus3:   |All points:|Prediction:\n");
		fprintf(logfile, "----------+----------+----------+----------+--------------+--------+----------+-----------+-----------\n");

		fclose(logfile);
		
	}
	
	if (status->timeleft > 0 && status->timeleft <= points_memory.lasttime - getConfigInt("points_log_interval"))
	{
		logfile = fopen(getConfigString("points_log_file"), "a");
		if (logfile == NULL)
		{
			wombatlog("Can't open file to log points", 5);
			return -1;
		}

		fprintf(logfile, "% 10i|%10f|%10f|%10f|%14f|% 8i|%10f|%11f|%11f\n", 
		    (int)status->timeleft, points_memory.autotrack,
		    points_memory.bonus1, points_memory.bonus2,
		    points_memory.bonus2solved, points_memory.bonus2howmanysolved, 
		    points_memory.bonus3, 
		    points_memory.from_lasttime_all, points_memory.prediction);
		fclose(logfile);

		points_memory.lasttime = status->timeleft;
		points_memory.autotrack = 0;
		points_memory.bonus1 = 0;
		points_memory.bonus2 = 0;
		points_memory.bonus2solved = 0;
		points_memory.bonus2howmanysolved=0;
		points_memory.bonus3 = 0;
		points_memory.from_lasttime_all = 0;
	}

	if (status->timeleft == 0)
	{
		logfile = fopen(getConfigString("points_log_file"), "a");
		if (logfile == NULL)
		{
			wombatlog("Can't open file to log points", 5);
			return -1;
		}

		fprintf(logfile, "Summary:  |%10f|%10f|%10f|%14f|% 8i|%10f|%11f|%11f\n", 
		    points_memory.autotrack_all,
		    points_memory.bonus1_all, points_memory.bonus2_all,
		    points_memory.bonus2solved_all, points_memory.bonus2howmanysolved_all,
		    points_memory.bonus3_all, 
		    points_memory.all, points_memory.prediction);
		fclose(logfile);
	}

	return 0;
}