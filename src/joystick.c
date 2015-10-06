#include "joystick.h"

int joystick_init()
{
	static SDL_Joystick **joysticks;
	int num;
	int i;

/*	num = SDL_NumJoysticks();
	
	for (i=0; i<num; i++)
	{
		SDL_JoystickClose(joysticks[i]);
	}

	free (joysticks);*/

	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	num = SDL_NumJoysticks();

	//if (num == 0)		//warning...
	
	joysticks = (SDL_Joystick **)malloc(num*sizeof(SDL_Joystick *));

	for (i=0; i<num; i++)
	{
		if (!SDL_JoystickOpened(i))
			joysticks[i] = SDL_JoystickOpen(i);
	}

	free(joysticks);

	return 0;
}

int joystick_load()
{
	int num;
	int i;	
	char filename[WOMBAT_FILENAME_SIZE];
	configentry joystick_names[JOYSTICKS_NR];

	config_createentry_string(&joystick_names[0], "right", "");
	config_createentry_string(&joystick_names[1], "left", "");

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "joystick_last", PACKAGE);
	configread(filename, joystick_names, JOYSTICKS_NR);

	Joy_Right_nr = -1;
	Joy_Left_nr = -1;

	num = SDL_NumJoysticks();

	for (i=0; i<num; i++)
	{
		if (strcmp(SDL_JoystickName(i), config_getentry_string(joystick_names, JOYSTICKS_NR, "right")) == 0 && Joy_Right_nr == -1)
		{
			Joy_Right_nr = i;
			if (joystick_load_configuration(1))
			{
				wombatlog("Right joystick not yet calibrated!", 5);
			}
		}
		if (strcmp(SDL_JoystickName(i), config_getentry_string(joystick_names, JOYSTICKS_NR, "left")) == 0)
		{
			Joy_Left_nr = i;
			if (joystick_load_configuration(2))
			{
				wombatlog("Left joystick not yet calibrated!", 5);
			}
		}
	}


	return 0;
}

int joystick_save()
{
	char filename[WOMBAT_FILENAME_SIZE];
	configentry joystick_names[JOYSTICKS_NR];

	config_createentry_string(&joystick_names[0], "right", (char *)SDL_JoystickName(Joy_Right_nr));
	config_createentry_string(&joystick_names[1], "left", (char *)SDL_JoystickName(Joy_Left_nr));

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "joystick_last", PACKAGE);
	configcreatedirforfile(filename);
	configwrite(filename, joystick_names, JOYSTICKS_NR);

	return 0;
}

int joystick_load_configuration(int which)
{
	char filename[WOMBAT_FILENAME_SIZE];
	FILE *joyconfigfile;
	joy_data joystick;
	int nr;

	switch (which)
	{
		case 1:
			nr = Joy_Right_nr;
			break;
		case 2:
			nr = Joy_Left_nr;
			break;
	}

	if (nr < 0)
	{
		return -1;
	}

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "joysticks", PACKAGE);
	joyconfigfile = fopen(filename, "rb");

	if (joyconfigfile == NULL)
	{
		return 1;
	}

	while (!feof(joyconfigfile))
	{
		if (fread(&joystick, sizeof(joy_data), 1, joyconfigfile) != 1)
			break;

		if (strcmp(SDL_JoystickName(nr), joystick.name) == 0)
		{
			switch (which)
			{
				case 1:	
					Joy_Right_button = joystick.button1;
					updateConfigInt("joy1_max", joystick.max);
					updateConfigInt("joy1_min", joystick.min);	
					break;
				case 2:
					Joy_Left_button = joystick.button1;
					updateConfigInt("joy2_max", joystick.max);
					updateConfigInt("joy2_min", joystick.min);
					break;
			}
			fclose (joyconfigfile);
			return 0;

		}
	}

	fclose(joyconfigfile);

	return 1;
}

int joystick_show_config()
{
	printf("joy1max: %i\njoy1min: %i\njoy2max: %i\n joy2min: %i\n", getConfigInt("joy1_max"), getConfigInt("joy1_min"), getConfigInt("joy2_max"), getConfigInt("joy2_min"));

	return 0;
}

int joystick_setup(ft_fontdata *fontdata)
{
	SDL_Event event;

	while (1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));
	
		drawText(getConfigInt("screen_w")/2-50, getConfigInt("screen_h")-100, 1, 1, 1, "Joystick configuration:", fontdata);
		drawText(50, getConfigInt("screen_h")-300, 1, 1, 1, "r - select right joystick", fontdata);
		drawText(50, getConfigInt("screen_h")-350, 1, 1, 1, "e - calibrate right joystick", fontdata);
		drawText(50, getConfigInt("screen_h")-400, 1, 1, 1, "l - select left joystick", fontdata);
		drawText(50, getConfigInt("screen_h")-450, 1, 1, 1, "k - calibrate left joystick", fontdata);
		drawText(getConfigInt("screen_w")-200, 50, 1, 1, 1, "ESC to quit", fontdata);
	
		SDL_GL_SwapBuffers();

		joystick_init();
		
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							joystick_save();
							return 0;
						break;

						case SDLK_r:
							joystick_select(1, fontdata);
						break;

						case SDLK_l:
							joystick_select(2, fontdata);
						break;

						case SDLK_e:
							joystick_calibration(1, fontdata);
						break;

						case SDLK_k:
							joystick_calibration(2, fontdata);
						break;
					}
				break;

			}
		}
	}


	joystick_save();
	return 0;
}

int joystick_setup_necessary(ft_fontdata *fontdata)
{
	if (Joy_Right_nr == -1)
	{
		joystick_init();
		if (joystick_select(1, fontdata) != 0)
			return 1;
	}
	if (Joy_Left_nr == -1)
	{
		joystick_init();
		if (joystick_select(2, fontdata) != 0)
			return 1;
	}

	if (joystick_load_configuration(1) > 0)
	{
		joystick_calibration(1, fontdata);
	}
	if (joystick_load_configuration(2) > 0)
	{
		joystick_calibration(2, fontdata);
	}

	joystick_save();

	return 0;
}

int joystick_select(int which, ft_fontdata *fontdata)
{	
	char text[40];
	SDL_Event event;
	//int finding=1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	switch (which)
	{
		case 1:
			sprintf(text, "Press Button 1 of RIGHT joystick");
			break;
		case 2:
			sprintf(text, "Press Button 1 of LEFT joystick");
			break;
	}
	drawText(getConfigInt("screen_w")/2-50, getConfigInt("screen_h")/2, 1, 1, 1, text, fontdata);

	SDL_GL_SwapBuffers();

	while (1)
	{
		joystick_init();
		SDL_Delay(500);
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							Joy_Right_nr = event.jaxis.which;
							Joy_Right_button = event.jbutton.button;
							if (joystick_load_configuration(1))
								joystick_calibration(1, fontdata);
							return 0;
							break;
						case 2:
							Joy_Left_nr = event.jaxis.which;
							Joy_Left_button = event.jbutton.button;
							if (joystick_load_configuration(2))
								joystick_calibration(2, fontdata);
							return 0;
							break;
					}
					break;
			}
		}
	}


	return 0;

}

int joystick_draw_calibration(int x1, int y1, int x2, int y2, char *text, ft_fontdata *fontdata)
{
	int w,h;

	w = getConfigInt("bonus1_rect_w")*5;
	h = getConfigInt("bonus1_rect_h")*5;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-getConfigInt("screen_w")/2, getConfigInt("screen_w")/2, -getConfigInt("screen_h")/2, getConfigInt("screen_h")/2);


	glBegin(GL_LINE_LOOP);	// Draw rectangle
		glColor3f(1.0f, 1.0f, 0.3f);
		glVertex2f(-w, h);
		glVertex2f(-w, -h);
		glVertex2f(w, -h);
		glVertex2f(w, h);
	glEnd();

	glBegin(GL_LINE_LOOP); // draw circle
		glColor3f(0.0f, 0.0f, 1.0f);
		drawCircle(x1, y1, 20, 20);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(x2-20, y2);
		glVertex2f(x2+20, y2);
		glVertex2f(x2, y2-20);
		glVertex2f(x2, y2+20);
	glEnd();

	drawText(50, getConfigInt("screen_h")-100, 1, 1, 1, text, fontdata);

	SDL_GL_SwapBuffers();	

	//SDL_Delay(1000);
}

int joystick_calibration(int which, ft_fontdata *fontdata)
{
	float x1, y1;
	int joyx, joyy;
	int waiting, time;

	char text[80];
	
	SDL_Joystick *joystick;
	SDL_Event event;

	joy_data myjoystick;

	int w,h;

	w = getConfigInt("bonus1_rect_w")*5;
	h = getConfigInt("bonus1_rect_h")*5;

	switch(which)
	{
		case 1:
			joystick = SDL_JoystickOpen(Joy_Right_nr);
			strncpy(myjoystick.name, SDL_JoystickName(Joy_Right_nr), JOY_NAME_SIZE);
			myjoystick.button1 = Joy_Right_button;
			break;
		case 2:
			joystick = SDL_JoystickOpen(Joy_Left_nr);
			strncpy(myjoystick.name, SDL_JoystickName(Joy_Left_nr), JOY_NAME_SIZE);
			myjoystick.button1 = Joy_Left_button;
			break;
	}

	if (joystick == NULL)
	{
		joystick_draw_calibration(0,0,0,0, "Joystick open failure!", fontdata);
		SDL_Delay(1000);
		return 1;
	}

	x1=0;
	y1=0;

	waiting =1;
	while (waiting)		// setting joy vertically
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								waiting=0;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								waiting=0;
							}
					}
			}
		}

		if (which == 1)
			strcpy(text, "RIGHT");
		if (which == 2)
			strcpy(text, "LEFT");
		strcat(text, " stick set vertically and press the button");
		joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, text, fontdata);
	}

	joyx=0;
	waiting =1;
	while (waiting)		// setting MINs
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								waiting=0;
								myjoystick.min = joyx;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								waiting=0;
								myjoystick.min = joyx; 
							}
							break;
					}
				case SDL_JOYAXISMOTION:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && abs(event.jaxis.value) > joyx)
							{
								joyx = abs(event.jaxis.value);
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && abs(event.jaxis.value) > joyx)
							{
								joyx = abs(event.jaxis.value);
							}
							break;
					}
			}
		}
	joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, "Wait a few seconds and press the button", fontdata);
	}

	time = SDL_GetTicks();
	joyx = 32767;
	joyy = 0;
	waiting =1;
	while (waiting)		// setting MAX right
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								joyx = joyy;
								waiting=0;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								joyx = joyy;
								waiting=0;
							}
					}
				case SDL_JOYAXISMOTION:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jaxis.axis == 0 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jaxis.axis == 0 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
					}
			}
		}
		if (x1 < w)
		{
			x1 += (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (x1 > w)
		{
			x1 = (float)w;
		}
		time = SDL_GetTicks();


		if (which == 1)
			strcpy(text, "RIGHT");
		if (which == 2)
			strcpy(text, "LEFT");
		strcat(text, " stick move right and press the button");
	joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, text, fontdata);
	}

	time = SDL_GetTicks();
	joyy = 0;
	waiting =1;
	while (waiting)		// setting MAX top
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
					}
				case SDL_JOYAXISMOTION:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jaxis.axis == 1 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jaxis.axis == 1 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
					}
			}
		}
		if (x1 > 0)
		{
			x1 -= (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (x1 < 0)
		{
			x1 = 0;
		}

		if (y1 < h)
		{
			y1 += (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (y1 > h)
		{
			y1 = h;
		}
		time = SDL_GetTicks();


		if (which == 1)
			strcpy(text, "RIGHT");
		if (which == 2)
			strcpy(text, "LEFT");
		strcat(text, " stick move forward and press the button");
		joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, text, fontdata);
	}

	time = SDL_GetTicks();
	joyy = 0;
	waiting =1;
	while (waiting)		// setting MAX bottom
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
					}
				case SDL_JOYAXISMOTION:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jaxis.axis == 1 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jaxis.axis == 1 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
					}
			}
		}

		if (x1 > 0)
		{
			x1 -= (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (x1 < 0)
		{
			x1 = 0;
		}
		if (y1 > -h)
		{
			y1 -= (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (y1 < -h)
		{
			y1 = -h;
		}
		time = SDL_GetTicks();


		if (which == 1)
			strcpy(text, "RIGHT");
		if (which == 2)
			strcpy(text, "LEFT");
		strcat(text, " stick move back and press the button");
		joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, text, fontdata);
	}

	time = SDL_GetTicks();
	joyy = 0;
	waiting =1;
	while (waiting)		// setting MAX left
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jbutton.button == Joy_Right_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jbutton.button == Joy_Left_button)
							{
								if (joyy < joyx)
									joyx = joyy;
								waiting=0;
							}
					}
				case SDL_JOYAXISMOTION:
					switch (which)
					{
						case 1:
							if (event.jaxis.which == Joy_Right_nr && event.jaxis.axis == 0 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
						case 2:
							if (event.jaxis.which == Joy_Left_nr && event.jaxis.axis == 0 && abs(event.jaxis.value) > joyy)
							{
								joyy = abs(event.jaxis.value);
							}
							break;
					}
			}
		}
		if (x1 > -w)
		{
			x1 -= (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (x1 < -w)
		{
			x1 = -w;
		}

		if (y1 < 0)
		{
			y1 += (float)JOY_CAL_V*(SDL_GetTicks()-time)/100;
		}
		if (y1 > 0)
		{
			y1 = 0;
		}
		time = SDL_GetTicks();


		if (which == 1)
			strcpy(text, "RIGHT");
		if (which == 2)
			strcpy(text, "LEFT");
		strcat(text, " stick move left and press the button");
		joystick_draw_calibration((int)x1, (int)y1, SDL_JoystickGetAxis(joystick, 0)*w/32767, -SDL_JoystickGetAxis(joystick, 1)*h/32767, text, fontdata);
	}

	myjoystick.max = joyx;

	joystick_append2file(myjoystick);
	if (joystick_load_configuration(which))
		return 1;

	return 0;
}

int joystick_append2file(joy_data joystick)
{
	char filename[WOMBAT_FILENAME_SIZE];
	FILE *joyconfigfile;
	joy_data joystick_read;  
	fpos_t position;


	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "joysticks", PACKAGE);
	configcreatedirforfile(filename);
	joyconfigfile = fopen(filename, "rb+");

	if (joyconfigfile == NULL)
	{
		joyconfigfile = fopen(filename, "wb");		//create new file
		if (joyconfigfile == NULL)
		{
			return 1;
		}

		if (fwrite(&joystick, sizeof(joy_data), 1, joyconfigfile) != 1)
		{
			fclose(joyconfigfile);
			return 1;
		}
		fclose(joyconfigfile);
		return 0;
	}

	while (!feof(joyconfigfile))
	{		
		printf("finding position inf file\n");
		fgetpos (joyconfigfile, &position);

		if (fread(&joystick_read, sizeof(joy_data), 1, joyconfigfile) != 1)
			break;

		if (strcmp(joystick_read.name, joystick.name) == 0)		// update position
		{
			fsetpos(joyconfigfile, &position);
			if (fwrite(&joystick, sizeof(joy_data), 1, joyconfigfile) != 1)
			{
				fclose(joyconfigfile);
				return 1;
			}
			fclose(joyconfigfile);
			return 0;
		}
	}

	printf("apend new position\n");

	fseek(joyconfigfile, 0, SEEK_END);
	if (fwrite(&joystick, sizeof(joy_data), 1, joyconfigfile) != 1)			// append new position
	{
		fclose(joyconfigfile);
		return 1;
	}
	fclose(joyconfigfile);

	return 0;
}
