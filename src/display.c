#include "display.h"

//extern int SCREEN_W;
//extern int SCREEN_H;

int initDisplay(SDL_Surface *screen)
{	
	const SDL_VideoInfo* info = NULL;	

	/* create SDL display */
	wombatlog("Creating SDL Display", 2);
	if(SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) == -1)
	{
		wombatlog("SDL Display not created", 10);
		return 1;
	}
	atexit(SDL_Quit);
	
	wombatlog("Getting display informations", 2);		
	info = SDL_GetVideoInfo();
	updateConfigInt("screen_w", info->current_w);
	updateConfigInt("screen_h", info->current_h);
	wombatlog("Setting configuration variables", 2);
	setGlobalVars();
	copyConfig2Vars();
	
	wombatlog("Setting openGL", 2);
	/* set openGL attributes */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, getConfigInt("screen_coldep"));
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, getConfigInt("screen_coldep"));
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, getConfigInt("screen_coldep"));
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, getConfigInt("screen_coldep"));
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );	

	wombatlog("Creating SDL / openGL draw context", 2);
	/* Create drawContext */	
	screen = SDL_SetVideoMode( getConfigInt("screen_w"), getConfigInt("screen_h"), 0, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);
	if(screen == NULL)
	{
		wombatlog("Draw context not created", 10);
		return 1;
	}
	
	SDL_WM_SetCaption ("WombatTraining", NULL);
	SDL_ShowCursor(SDL_DISABLE);
	
	wombatlog("Initializing openGL scene", 2	);
	/* OpenGL initialization */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	return 0;
}

int drawGLScene(wombatGameStatus *status, ft_fontdata *fontdata)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));
	
	if (status->ifbonus)
	{
		drawBonus(status, fontdata);
		/*Draw bonus screen*/
	}
	else 	//Draw main task
	{
		drawMainTask(status);
	}	
	
	drawTopScreen(status, fontdata);
	drawFPS(status, fontdata);
	
	SDL_GL_SwapBuffers();	
	
	return 0;
}

int drawMainTask(wombatGameStatus *status)
{
	float joyix, joyiy, joyiz;		// relative joystick indicator
	
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	/* Drawing the cross */
	glBegin(GL_LINES);
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	glVertex2f((float)status->x.cat-getConfigInt("cross_w"), (float)status->y.cat);
	glVertex2f((float)status->x.cat+getConfigInt("cross_w"), (float)status->y.cat);
	glVertex2f((float)status->x.cat, (float)status->y.cat-getConfigInt("cross_h"));
	glVertex2f((float)status->x.cat, (float)status->y.cat+getConfigInt("cross_h"));
	glEnd();
	/* Draw joystick indicator */
	if (abs(status->x.joy) > getConfigInt("joy1_min"))
		joyix=((float)status->x.joy-getConfigInt("joy1_min"))*getConfigInt("joyind_scale")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
	else 
		joyix=0;
	if (abs(status->y.joy) > getConfigInt("joy1_min"))
		joyiy=((float)status->y.joy-getConfigInt("joy1_min"))*getConfigInt("joyind_scale")/(getConfigInt("joy1_max")-getConfigInt("joy1_min"));
	else 
		joyiy=0;

	glBegin(GL_LINES);	
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	glVertex2f(status->x.cat, status->y.cat);
	glVertex2f(status->x.cat+(float)joyix, status->y.cat+(float)joyiy);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	drawCircle(status->x.cat+joyix, status->y.cat+joyiy, (float)getConfigInt("joyind_r"), getConfigInt("joyind_l"));
	glEnd();
	
	/* Drawing the circle */
	glBegin(GL_LINE_LOOP);
	glColor3f(getConfigFloat("circle_cr"), getConfigFloat("circle_cg"), getConfigFloat("circle_cb"));
	drawCircle(status->x.m, status->y.m, (float)getConfigInt("circle_r"), getConfigInt("circle_l"));
	glEnd();
	
	/* Drawing the hexagon */
	glBegin(GL_LINE_LOOP);
	glColor3f(getConfigFloat("circle_cr"), getConfigFloat("circle_cg"), getConfigFloat("circle_cb"));
	glVertex2f(status->z.m, getConfigInt("taskarea_h")/2-(getConfigInt("taskarea_h")/2-status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"))/2);
	glVertex2f(getConfigInt("taskarea_w")/2, status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"));
	glVertex2f(getConfigInt("taskarea_w")-status->z.m-1, getConfigInt("taskarea_h")/2-(getConfigInt("taskarea_h")/2-status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"))/2);
	glVertex2f(getConfigInt("taskarea_w")-status->z.m-1, getConfigInt("taskarea_h")-1-getConfigInt("taskarea_h")/2+(getConfigInt("taskarea_h")/2-status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"))/2);
	glVertex2f(getConfigInt("taskarea_w")/2, getConfigInt("taskarea_h")-status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"));
	glVertex2f(status->z.m, getConfigInt("taskarea_h")-1-getConfigInt("taskarea_h")/2+(getConfigInt("taskarea_h")/2-status->z.m*getConfigInt("taskarea_h")/getConfigInt("taskarea_w"))/2);
	glEnd();
	
	/* Drawing vertical lines */
	glBegin(GL_LINES);
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	glVertex2f(status->z.cat-getConfigInt("lines_s"), getConfigInt("taskarea_h")/2+getConfigInt("lines_h"));
	glVertex2f(status->z.cat-getConfigInt("lines_s"), getConfigInt("taskarea_h")/2-getConfigInt("lines_h"));
	glVertex2f(status->z.cat+getConfigInt("lines_s"), getConfigInt("taskarea_h")/2+getConfigInt("lines_h"));
	glVertex2f(status->z.cat+getConfigInt("lines_s"), getConfigInt("taskarea_h")/2-getConfigInt("lines_h"));
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat-getConfigInt("lines_s"), getConfigInt("taskarea_h")/2+getConfigInt("lines_h"));
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat-getConfigInt("lines_s"), getConfigInt("taskarea_h")/2-getConfigInt("lines_h"));
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat+getConfigInt("lines_s"), getConfigInt("taskarea_h")/2+getConfigInt("lines_h"));
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat+getConfigInt("lines_s"), getConfigInt("taskarea_h")/2-getConfigInt("lines_h"));
	glEnd();
	/* Drawing joystick indicator */
	if (abs(status->z.joy) > getConfigInt("joy2_min"))
		joyiz=(status->z.joy-getConfigInt("joy2_min"))*getConfigInt("joyind_scale")/(getConfigInt("joy2_max")-getConfigInt("joy2_min"));
	else 
		joyiz=0;
	glBegin(GL_LINES);	
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	glVertex2f(status->z.cat, getConfigInt("taskarea_h")/2);
	glVertex2f(status->z.cat+joyiz, getConfigInt("taskarea_h")/2);
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat, getConfigInt("taskarea_h")/2);
	glVertex2f(getConfigInt("taskarea_w")-status->z.cat-joyiz, getConfigInt("taskarea_h")/2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	drawCircle(status->z.cat+joyiz, (float)getConfigInt("taskarea_h")/2, (float)getConfigInt("joyind_r"), getConfigInt("joyind_l"));
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(getConfigFloat("cross_cr"), getConfigFloat("cross_cg"), getConfigFloat("cross_cb"));
	drawCircle((float)getConfigInt("taskarea_w")-status->z.cat-joyiz, (float)getConfigInt("taskarea_h")/2, (float)getConfigInt("joyind_r"), getConfigInt("joyind_l"));
	glEnd();

	return 0;
}

int drawTopScreen(wombatGameStatus *status, ft_fontdata *fontdata)
{		
	float color;
	
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));


	// Left points indicator
	glColor3f(getConfigFloat("points_m_cr"), getConfigFloat("points_m_cg"), getConfigFloat("points_m_cb"));
	glBegin(GL_POLYGON);
	glVertex2f(0, getConfigInt("screen_h"));
	glVertex2f(0, getConfigInt("screen_h") - getConfigInt("points_h"));
	glVertex2f(getConfigInt("points_w"), getConfigInt("screen_h") - getConfigInt("points_h"));
	glVertex2f(getConfigInt("points_w"), getConfigInt("screen_h"));
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_border"));
	glVertex2f(getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border"));
	glVertex2f(getConfigInt("points_w")-getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border"));
	glVertex2f(getConfigInt("points_w")-getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_border"));
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(getConfigInt("points_border")+1, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1);
	glVertex2f(getConfigInt("points_border")+1+status->pmvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1);
	glVertex2f(getConfigInt("points_border")+1+status->pmvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1+status->pmperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
	glVertex2f(getConfigInt("points_border")+1, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1+status->pmperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
	glEnd();

	if (status->failure && status->mode>0)
	{
		color = 1.0-(float)status->mode/getConfigInt("at_maxperformance")/1.5;
		glColor3f(color, color, color);
		glBegin(GL_POLYGON);
		glVertex2f(getConfigInt("points_border")+2, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+2);
		glVertex2f(getConfigInt("points_border")+status->pmvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+2);
		glVertex2f(getConfigInt("points_border")+status->pmvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+status->pmperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
		glVertex2f(getConfigInt("points_border")+2, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+status->pmperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
		glEnd();
	}

	if (status->failure && status->mode <=0)
	{
		drawText(getConfigInt("points_w")+50, getConfigInt("screen_h")-30, 1.0, 0.0, 0.0, "Autotrack failure", fontdata);
	}	


	// Right points indicator
	glColor3f(getConfigFloat("points_b_cr"), getConfigFloat("points_b_cg"), getConfigFloat("points_b_cb"));
	glBegin(GL_POLYGON);
	glVertex2f(getConfigInt("screen_w"), getConfigInt("screen_h"));
	glVertex2f(getConfigInt("screen_w"), getConfigInt("screen_h") - getConfigInt("points_h"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w"), getConfigInt("screen_h") - getConfigInt("points_h"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w"), getConfigInt("screen_h"));
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_border"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border"), getConfigInt("screen_h")-getConfigInt("points_border"));
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border")+1, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border")+1+status->pbvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border")+1+status->pbvalue*(getConfigInt("points_w")-2*getConfigInt("points_border")-2)/getConfigInt("points_value_max"), getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1+status->pbperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_w")+getConfigInt("points_border")+1, getConfigInt("screen_h")-getConfigInt("points_h")+getConfigInt("points_border")+1+status->pbperformance*(getConfigInt("points_h")-2*getConfigInt("points_border")-2)/getConfigInt("points_performance_max"));
	glEnd();


	// points bar
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(getConfigInt("screen_w")-1, 1);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w")-1, 1);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w")-1, getConfigInt("points_bar_h")+1);
	glVertex2f(getConfigInt("screen_w")-1, getConfigInt("points_bar_h")+1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(getConfigInt("screen_w")-1, (getConfigInt("points_bar_h")-2)*status->ppredicted/getConfigInt("points_bar_max")+2);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w")-1, (getConfigInt("points_bar_h")-2)*status->ppredicted/getConfigInt("points_bar_max")+2);
	glEnd();

	glColor3f(getConfigFloat("points_bar_cr"), getConfigFloat("points_bar_cg"), getConfigFloat("points_bar_cb"));
	glBegin(GL_POLYGON);
	glVertex2f(getConfigInt("screen_w")-2, 2);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w"), 2);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w"), (getConfigInt("points_bar_h")-2)*status->ppoints/getConfigInt("points_bar_max")+2);
	glVertex2f(getConfigInt("screen_w")-2, (getConfigInt("points_bar_h")-2)*status->ppoints/getConfigInt("points_bar_max")+2);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(getConfigInt("screen_w")-2, (getConfigInt("points_bar_h")-2)*status->ppoints/getConfigInt("points_bar_max")+2);
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w")/2-1, (getConfigInt("points_bar_h")-2)*status->ppoints/getConfigInt("points_bar_max")+2+getConfigFloat("points_triangle_height")*(status->ptable[0][1]-status->ptable[(int)((POINTS_TABLE_SIZE-1)/getConfigFloat("points_triangle_timeD"))][1])/(status->ptable[(int)((POINTS_TABLE_SIZE-1)/getConfigFloat("points_triangle_timeD"))][0]-status->ptable[0][0]));
	glVertex2f(getConfigInt("screen_w")-getConfigInt("points_bar_w"), (getConfigInt("points_bar_h")-2)*status->ppoints/getConfigInt("points_bar_max")+2);
	glEnd();
	
/*	if(status->failure)							// failure
	{
		glColor3f(FAIL_CR, FAIL_CG, FAIL_CB);
		glBegin(GL_POLYGON);
		glVertex2f(FAIL_X, FAIL_Y);
		glVertex2f(FAIL_X, FAIL_Y-FAIL_H);
		glVertex2f(FAIL_X+FAIL_W, FAIL_Y-FAIL_H);
		glVertex2f(FAIL_X+FAIL_W, FAIL_Y);
		glEnd();
	}*/
	
/*	if(status->bonusresult != 0)		// basic version of bonus result
	{
		if (status->bonusresult>0)
			glColor3f(VICT_CR, VICT_CG, VICT_CB);
		else 		
			glColor3f(FAIL_CR, FAIL_CG, FAIL_CB);
			
		glBegin(GL_POLYGON);
		glVertex2f(VICT_X, VICT_Y);
		glVertex2f(VICT_X, VICT_Y-VICT_H);
		glVertex2f(VICT_X+VICT_W, VICT_Y-VICT_H);
		glVertex2f(VICT_X+VICT_W, VICT_Y);
		glEnd();
	}*/

	return 0;
}

int drawFPS(wombatGameStatus *status, ft_fontdata *fontdata)
{
	static int lasttime;
	char text[20];
	
	sprintf(text, "fps: %f", (float)1000/(SDL_GetTicks()-lasttime));
	lasttime = SDL_GetTicks();
	drawText(getConfigInt("screen_w")/2-50, getConfigInt("screen_h")-30, 0.5, 0.5, 0.5, text, fontdata);

	return 0;
}
	


int drawBonus(wombatGameStatus *status, ft_fontdata *fontdata)
{
	if (status->whichbonus == 0)
	{
		drawSelectBonus(status, fontdata);
	}
	else 
		drawClock(status);
	
	if (status->whichbonus == 1)
		drawBonus1(status, fontdata);
		
	if (status->whichbonus == 2)
		drawBonus2(status, fontdata);	
		
	if (status->whichbonus == 3)
	{
		drawBonus3(status, fontdata);
	}
	
	return 0;
}

int drawMenu(wombatGameStatus *status, ft_fontdata *fontdata, char *message)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	drawText(getConfigInt("screen_w")/2-50, getConfigInt("screen_h")-100, 1, 1, 1, "Main menu:", fontdata);
	drawText(50, getConfigInt("screen_h")-300, 1, 1, 1, "j - set joysticks", fontdata);
	if (Joy_Right_nr == -1 || Joy_Left_nr == -1)
		drawText(350, getConfigInt("screen_h")-300, 1, 1, 1, "(you should!)", fontdata);	
	drawText(50, getConfigInt("screen_h")-350, 1, 1, 1, "s - start/resume game", fontdata);
	drawText(50, getConfigInt("screen_h")-400, 1, 1, 1, "l - load game", fontdata);
	drawText(50, getConfigInt("screen_h")-450, 1, 1, 1, "k - save game", fontdata);
	drawText(getConfigInt("screen_w")-200, 50, 1, 1, 1, "ESC to quit", fontdata);

	drawText(getConfigInt("screen_w")/2-200, getConfigInt("screen_h")-550, 1, 1, 1, message, fontdata);

	SDL_GL_SwapBuffers();	
	
	return 0;

}

int drawBonusValueRect(wombatGameStatus *status, int top, int value)
{
	int height=10;
	int width=60;
	int x=getConfigInt("taskarea_w")/2;
	int valuewidth;
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	// Border
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, top);
	glVertex2f(x, top-height);
	glVertex2f(x+width, top-height);
	glVertex2f(x+width, top);
	glEnd();

	// Value
	valuewidth = (width-2)*value/getConfigInt("points_value_max");

	
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(x+1, top-1);
	glVertex2f(x+1, top-height+1);
	glVertex2f(x+valuewidth+1, top-height+1);
	glVertex2f(x+valuewidth+1, top-1);
	glEnd();

	return 0;
}

int drawSelectBonus(wombatGameStatus *status, ft_fontdata *fontdata)
{
	drawText(getConfigInt("screen_w")/2-100, getConfigInt("screen_h")-100, 1, 1, 1, "Select Bonus", fontdata);
	drawText(100, getConfigInt("screen_h")-300, 1, 1, 1, "1: 3D Shapes", fontdata);
	drawBonusValueRect(status, getConfigInt("screen_h")-300, status->bonusvalue[0]);
	drawText(100, getConfigInt("screen_h")-400, 1, 1, 1, "q: Funding numbers", fontdata);
	drawBonusValueRect(status, getConfigInt("screen_h")-400, status->bonusvalue[1]);
	drawText(100, getConfigInt("screen_h")-500, 1, 1, 1, "a: Remember digits", fontdata);
	drawBonusValueRect(status, getConfigInt("screen_h")-500, status->bonusvalue[2]);
	
	return 0;
}

int drawClock(wombatGameStatus *status)
{	
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(getConfigFloat("clock_b_cr"), getConfigFloat("clock_b_cg"), getConfigFloat("clock_b_cb"));
	glVertex2f(getConfigInt("clock_x"), getConfigInt("clock_y"));
	drawCircle((float)getConfigInt("clock_x"), (float)getConfigInt("clock_y"), (float)getConfigInt("clock_r"), getConfigInt("clock_l"));
	glEnd();
		
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(getConfigFloat("clock_f_cr"), getConfigFloat("clock_f_cg"), getConfigFloat("clock_f_cb"));
	glVertex2f(getConfigInt("clock_x"), getConfigInt("clock_y"));
	drawPartCircle((float)getConfigInt("clock_x"), (float)getConfigInt("clock_y"), (float)getConfigInt("clock_r"), getConfigInt("clock_l"), ((float)getConfigInt("bonus_time")-status->bonustimeleft)*360/getConfigInt("bonus_time"));
	glEnd();
	
	return 0;
}

void tcbVertex(GLvoid *vertex)
{
		GLdouble *ptr;
		
		ptr = (GLdouble *) vertex;
		glVertex3dv((GLdouble *)ptr);
		glColor3dv((GLdouble *)ptr+3);
}

int drawBonus1(wombatGameStatus *status, ft_fontdata *fontdata)
{
	int x, i, j;
	GLUtesselator *tess = gluNewTess();
	GLdouble ***coords;
	
	glLoadIdentity();
	glOrtho(-64.0f, 64.0f, -40.0f, 40.0f, -40.0f, 40.0f);
	
	if(status->bonus1active)	
		x = 30;
	else 
		x = -34;
	
	gluTessCallback (tess, GLU_TESS_BEGIN, &glBegin); 
	gluTessCallback (tess, GLU_TESS_VERTEX, &tcbVertex); 
	gluTessCallback (tess, GLU_TESS_END, &glEnd);	
	gluTessProperty (tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD); 
	
	glBegin(GL_LINE_LOOP);	// Draw rectangle
		glColor3f(1.0f, 1.0f, 0.3f);
		glVertex2f(x-getConfigInt("bonus1_rect_w"), getConfigInt("bonus1_rect_h"));
		glVertex2f(x-getConfigInt("bonus1_rect_w"), -getConfigInt("bonus1_rect_h"));
		glVertex2f(x+getConfigInt("bonus1_rect_w"), -getConfigInt("bonus1_rect_h"));
		glVertex2f(x+getConfigInt("bonus1_rect_w"), getConfigInt("bonus1_rect_h"));
	glEnd();
	
	glTranslatef(-34,0.0,0);
	glRotatef(status->bonus11rotx,1.0f,0.0f,0.0f);
	glRotatef(status->bonus11roty,0.0f,1.0f,0.0f);
	glRotatef(status->bonus11rotz,0.0f,0.0f,1.0f);	

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST); 
	
	coords = (GLdouble ***)malloc(bonus1fig[status->bonus1fig1nr].wallsnumber*sizeof(GLdouble **));		// Hide some lines
	for (i=0; i<bonus1fig[status->bonus1fig1nr].wallsnumber; i++) 
	{
		gluTessBeginPolygon (tess, NULL); 
		gluTessBeginContour (tess);
		
		coords[i] = (GLdouble **)malloc(bonus1fig[status->bonus1fig1nr].wall[i].vertexnumber*sizeof(GLdouble *));	
		
		for(j=0; j<bonus1fig[status->bonus1fig1nr].wall[i].vertexnumber; j++)
		{
			coords[i][j] = (GLdouble *)malloc(6*sizeof(GLdouble));	
			
			coords[i][j][0] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].x;
			coords[i][j][1] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].y;
			coords[i][j][2] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].z;
			coords[i][j][3] = 1.0f;
			coords[i][j][4] = 1.0f;
			coords[i][j][5] = 0.0f;
			gluTessVertex (tess, coords[i][j], coords[i][j]); 

		}
		
		gluTessEndContour (tess); 
		gluEndPolygon (tess);			
	}	
	
	for (i=0; i < bonus1fig[status->bonus1fig1nr].wallsnumber; i++) 
	{
		for(j=0; j<bonus1fig[status->bonus1fig1nr].wall[i].vertexnumber; j++)
			free(coords[i][j]);
		free(coords[i]);
	}
	free (coords);
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPolygonOffset(-0.5,3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw only lines
	glEnable(GL_POLYGON_OFFSET_LINE);

	for (i=0; i < bonus1fig[status->bonus1fig1nr].wallsnumber; i++)	// Draw lines
	{
		glBegin(GL_POLYGON);
		glColor3f(1.0f,1.0f,0.3f);	
		
		for (j=0; j < bonus1fig[status->bonus1fig1nr].wall[i].vertexnumber; j++)
		{
			glVertex3f( getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].x , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].y , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig1nr].wall[i].vertex[j].z );
		}
		
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
	
	
	/* Figure right*/
	glLoadIdentity();
	glOrtho(-64.0f, 64.0f, -40.0f, 40.0f, -40.0f, 40.0f);
	
	glTranslatef(30,0.0,0);
	glRotatef(status->bonus12rotx,1.0f,0.0f,0.0f);
	glRotatef(status->bonus12roty,0.0f,1.0f,0.0f);
	glRotatef(status->bonus12rotz,0.0f,0.0f,1.0f);	

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST); 
	
	coords = (GLdouble ***)malloc(bonus1fig[status->bonus1fig2nr].wallsnumber*sizeof(GLdouble **));		// Hide some lines
	for (i=0; i<bonus1fig[status->bonus1fig2nr].wallsnumber; i++) 
	{
		gluTessBeginPolygon (tess, NULL); 
		gluTessBeginContour (tess);
		
		coords[i] = (GLdouble **)malloc(bonus1fig[status->bonus1fig2nr].wall[i].vertexnumber*sizeof(GLdouble *));	
		
		for(j=0; j<bonus1fig[status->bonus1fig2nr].wall[i].vertexnumber; j++)
		{
			coords[i][j] = (GLdouble *)malloc(6*sizeof(GLdouble));	
			
			if (status->bonus1mode == 1 || status->bonus1mode == 3)		// symetric
				coords[i][j][0] = -getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].x;
			else
				coords[i][j][0] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].x;
			coords[i][j][1] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].y;
			coords[i][j][2] = getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].z;
			coords[i][j][3] = 1.0f;
			coords[i][j][4] = 1.0f;
			coords[i][j][5] = 0.0f;
			gluTessVertex (tess, coords[i][j], coords[i][j]); 

		}
		
		gluTessEndContour (tess); 
		gluEndPolygon (tess);			
	}	
	
	for (i=0; i < bonus1fig[status->bonus1fig2nr].wallsnumber; i++) 
	{
		for(j=0; j<bonus1fig[status->bonus1fig2nr].wall[i].vertexnumber; j++)
			free(coords[i][j]);
		free(coords[i]);
	}
	free (coords);
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPolygonOffset(-0.5,0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw only lines
	glEnable(GL_POLYGON_OFFSET_LINE);

	for (i=0; i < bonus1fig[status->bonus1fig2nr].wallsnumber; i++)	// Draw lines
	{
		glBegin(GL_POLYGON);
		glColor3f(1.0f,1.0f,0.3f);	
		
		for (j=0; j < bonus1fig[status->bonus1fig2nr].wall[i].vertexnumber; j++)
		{
			if(status->bonus1mode == 1 || status->bonus1mode == 3)
				glVertex3f( -getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].x , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].y , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].z );
			else
				glVertex3f( getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].x , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].y , getConfigFloat("bonus1_fig_size")*bonus1fig[status->bonus1fig2nr].wall[i].vertex[j].z );
		}
		
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
	
	gluDeleteTess(tess);
	
	drawText(50, 100, 1, 1, 1, "1: identiczne", fontdata);
	drawText(50, 60, 1, 1, 1, "6: symetriczne mirror", fontdata);
	drawText(50, 20, 1, 1, 1, "3: completnie different", 	fontdata);
	
	return 0;
}

int drawBonus2(wombatGameStatus *status, ft_fontdata *fontdata)
{
	int i;
	int x, y;
	char *number;
	number = (char *)malloc(3*sizeof(char));
	
	/* Draw the cross */
	glBegin(GL_LINES);
	glColor3f(1, 1, 0.3);
	glVertex2f(0, getConfigInt("taskarea_h")/2);
	glVertex2f(getConfigInt("taskarea_w"), getConfigInt("taskarea_h")/2);
	glVertex2f(getConfigInt("taskarea_w")/2, 0);
	glVertex2f(getConfigInt("taskarea_w")/2, getConfigInt("taskarea_h"));
	glEnd();
	
	for (i = status->bonus2whichnr; i < getConfigInt("bonus2_numbers"); i++)
	{
		/* Choose correct rectangle */
		if (status->bonus2table[i][0] == 1 || status->bonus2table[i][0] == 2)
			y = getConfigInt("taskarea_h")/2+10;
		else 
			y = 10;
		
		if (status->bonus2table[i][0] == 2 || status->bonus2table[i][0] == 4)
			x = getConfigInt("taskarea_w")/2+10;
		else 
			x = 10;
		
		/* prepare character string */
		*number = ' ';
		if (i >= 9)
			*number = '1';
		if (i >= 19)
			*number = '2';
		if (i >= 29)
			*number = '3';
			
		if (i%10 == 9)
			*(number+sizeof(char)) = '0';
		else 
			*(number+sizeof(char)) = i%10+49;
			
		*(number+2*sizeof(char)) = '\0';
		
		/* Show number */
		drawText(x+status->bonus2table[i][1]*getConfigInt("bonus2_cell_w"), y+status->bonus2table[i][2]*getConfigInt("bonus2_cell_h"), getConfigFloat("bonus2_n_cr"), getConfigFloat("bonus2_n_cg"), getConfigFloat("bonus2_n_cb"), number, fontdata);		
	}
	
	free(number);
	return 0;
}

int drawBonus3(wombatGameStatus *status, ft_fontdata *fontdata)
{
	char *cyfra;
	cyfra = (char *)malloc(2*sizeof(char));

	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));	
	
	if (status->bonus3wfi)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(getConfigFloat("bonus3_sq_cr"), getConfigFloat("bonus3_sq_cg"), getConfigFloat("bonus3_sq_cb"));
		glVertex2f(getConfigInt("taskarea_w")/2, getConfigInt("taskarea_h")/2-3*getConfigInt("bonus3_sq"));
		glVertex2f(getConfigInt("taskarea_w")/2, getConfigInt("taskarea_h")/2-2*getConfigInt("bonus3_sq"));
		glVertex2f(getConfigInt("taskarea_w")/2+getConfigInt("bonus3_sq"), getConfigInt("taskarea_h")/2-2*getConfigInt("bonus3_sq"));
		glVertex2f(getConfigInt("taskarea_w")/2+getConfigInt("bonus3_sq"), getConfigInt("taskarea_h")/2-3*getConfigInt("bonus3_sq"));
		glEnd();
	}
	
	if (status->bonus3digit > 0)
	{
		glBegin(GL_QUADS);
		switch(status->bonus3digit) {
			case 1:
			case 2:
			case 5:
			case 6:
				glColor3f(1, 1, 1);
				break;
			case 3:
			case 4:
			case 7:
			case 8:
				glColor3f(1, 1, 0.3);
				break;
		}
		glVertex2f(getConfigInt("taskarea_w")/2-5, getConfigInt("taskarea_h")/2-5);
		glVertex2f(getConfigInt("taskarea_w")/2-5, getConfigInt("taskarea_h")/2+getConfigInt("bonus3_sq")+5);
		glVertex2f(getConfigInt("taskarea_w")/2+getConfigInt("bonus3_sq")+5, getConfigInt("taskarea_h")/2+getConfigInt("bonus3_sq")+5);
		glVertex2f(getConfigInt("taskarea_w")/2+getConfigInt("bonus3_sq")+5, getConfigInt("taskarea_h")/2-5);
		glEnd();
		
		cyfra[0] = status->bonus3digit+48;
		*(cyfra+sizeof(char)) = '\0';
		drawText(getConfigInt("taskarea_w")/2, getConfigInt("taskarea_h")/2, getConfigFloat("bonus3_f_cr"), getConfigFloat("bonus3_f_cg"), getConfigFloat("bonus3_f_cb"), cyfra, fontdata);
	}

	free(cyfra);	
	
	return 0;
}
