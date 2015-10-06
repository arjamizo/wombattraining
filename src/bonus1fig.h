#include "vars.h"
#include "logging.h"

#ifndef BONUS1FIG_H
# define BONUS1FIG_H

typedef struct wombatB1Vertex {
	float x, y, z;
} wombatB1Vertex;

typedef struct wombatB1Wall {
	int vertexnumber;
	wombatB1Vertex *vertex;
} wombatB1Wall;

typedef struct wombatB1Figure {
	int wallsnumber;
	wombatB1Wall *wall;
} wombatB1Figure;

wombatB1Figure *bonus1fig;

#endif

int Bonus1RemoveFigures();
int Bonus1ReadFiguresFromFile(char *filename);
int Bonus1ReadFigures(char *scenario_name);
int Bonus1SaveFigures();