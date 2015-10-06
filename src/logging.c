#include "logging.h"

int wombatlog(char *text, int status)
{
	char level;

	switch (status) {
		case 1: level = 'a'; break;
		case 2: level = 'i'; break;
		case 5: level = 'W'; break;
		case 10: level = 'A'; break;
		default: level = '?';
	}

	if (status >= getConfigInt("loglevel"))
	{
		printf("[%c] %s\n", level, text);
	}
	
	return 0;
}
