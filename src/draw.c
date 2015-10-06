#include "draw.h"

int drawCircle(float x, float y, float r, int lines)
{
	return drawPartCircle(x, y, r, lines, 360);
}

int drawPartCircle(float x, float y, float r, int lines, float angledeg)
{
	float i;
	float step = 2.0*M_PI/lines;
	
	for (i=0; i<M_PI*angledeg/180+step; i+=step)
	{
		//printf("Petla nr %f wspolrzedne: x=%f y=%f	\n", i, x, y);
		glVertex2f(x+r*sin(i), y+r*cos(i));
	}
	
	return 0;
}