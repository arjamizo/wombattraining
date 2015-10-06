#include "vars.h"
#include "logging.h"
#include "wombat.h"

#ifndef SCENARIO_H
# define SCENARIO_H

#define SCENARIO_EVENT_BUFFSIZE 24

typedef struct scenario_event scenario_event;

struct scenario_event {
	int when;
	unsigned int what;
	int parameter;
};

int scenario_check_name(char *scenario_name);
int check_scenario_path(char *scenario_path);
int scenario_load_event(unsigned int nr);
int scenario_load_game_events(unsigned int nr, wombatGameStatus *status, wombatTimer *timer);
int scenario_event_check(int time, unsigned int nr, wombatGameStatus *status, wombatTimer *timer);
int scenario_event_exe(wombatGameStatus *status, wombatTimer *timer);

#endif