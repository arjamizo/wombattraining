#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "configfile.h"
#include "vars.h"
#include "events.h"
#include "font.h"
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define JOYSTICKS_NR 2
#define JOY_CAL_V 10
#define JOY_NAME_SIZE 30

typedef struct {
	char name[JOY_NAME_SIZE];
	int min, max;
	int button1;
} joy_data;

int menu_joystick_init();
int menu_joystick_load();
int joystick_load_configuration(int which);
int joystick_setup(ft_fontdata *fontdata);
int joystick_setup_necessary(ft_fontdata *fontdata);
int joystick_draw_calibration(int x1, int y1, int x2, int y2, char *text, ft_fontdata *fontdata);
int joystick_calibration(int which, ft_fontdata *fontdata);
int joystick_append2file(joy_data joystick);

#endif
