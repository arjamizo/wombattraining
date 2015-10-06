#include "bonus1fig.h"

int Bonus1RemoveFigures()
{
	int i, j;
	for (i=0; i<getConfigInt("bonus1_fig_nr"); i++)
	{
			for (j=0; j< bonus1fig[i].wallsnumber; j++)
			{
				free(bonus1fig[i].wall[j].vertex);
			} 
	}
	
	free(bonus1fig);
	
	updateConfigInt("bonus1_fig_nr", 0);
	
	return 0;
}

int Bonus1ReadFiguresFromFile(char *filename)
{
	int i=getConfigInt("bonus1_fig_nr"), j, k, l;
	int ifidentical;
	char tmp[160];
	FILE *file_figures;

	file_figures = fopen(filename, "r");
	if (file_figures == NULL)
	{
		sprintf(tmp, "Can't open file %s to load figures", filename);
		wombatlog(tmp, 5);
		return 1;
	}

	while(fscanf(file_figures, "%i", &j) == 1)
	{
		bonus1fig = (wombatB1Figure *)realloc(bonus1fig, ++i*sizeof(wombatB1Figure));

		bonus1fig[i-1].wallsnumber = j;
		bonus1fig[i-1].wall = (wombatB1Wall *)malloc(j*sizeof(wombatB1Wall));
		
		for (j=0; j<bonus1fig[i-1].wallsnumber; j++)
		{	
			if (fscanf(file_figures, "%i", &k) != 1)
			{
				wombatlog("Error in config for bonus1", 5);
				return 1;
			}
			bonus1fig[i-1].wall[j].vertexnumber=k;
			bonus1fig[i-1].wall[j].vertex = (wombatB1Vertex *)malloc(k*sizeof(wombatB1Vertex));
			
			for (k=0; k<bonus1fig[i-1].wall[j].vertexnumber; k++)
			{
				if (fscanf(file_figures, "%f/%f/%f", &(bonus1fig[i-1].wall[j].vertex[k].x), &(bonus1fig[i-1].wall[j].vertex[k].y), &(bonus1fig[i-1].wall[j].vertex[k].z)) != 3)
				{
					wombatlog("Error in config for bonus1", 5);
					return 1;
				}
			}
		}
		if (i == 1)
		{
			ifidentical=0;
		}
		else {
			ifidentical=1;
		}
		for (j=0; j<i-1; j++)		// compare new figure to any previous one (to avoid duplicates)
		{
			if (bonus1fig[i-1].wallsnumber == bonus1fig[j].wallsnumber)
			{
				for (k=0; k<bonus1fig[i-1].wallsnumber; k++)
				{
					if (bonus1fig[i-1].wall[k].vertexnumber == bonus1fig[j].wall[k].vertexnumber)
					{
						for (l=0; l<bonus1fig[i-1].wall[k].vertexnumber; l++)
						{
							if (bonus1fig[i-1].wall[k].vertex[l].x != bonus1fig[j].wall[k].vertex[l].x ||
							    bonus1fig[i-1].wall[k].vertex[l].y != bonus1fig[j].wall[k].vertex[l].y ||
							    bonus1fig[i-1].wall[k].vertex[l].z != bonus1fig[j].wall[k].vertex[l].z)
							{
								ifidentical=0;
							}
						}
					}
					else {
						ifidentical = 0;
					}
				}
			}
			else {
				ifidentical = 0;
			}
		}

		if (ifidentical)
		{
			bonus1fig = (wombatB1Figure *)realloc(bonus1fig, --i*sizeof(wombatB1Figure));
		}
			
								
	}
	updateConfigInt("bonus1_fig_nr", i);

	sprintf(tmp, "Loaded %i figures", getConfigInt("bonus1_fig_nr"));
	wombatlog(tmp, 2);

	fclose(file_figures);
	return 0;
}

int Bonus1ReadFigures(char *scenario_name)
{
	int i=0, j, k;
	char filename[WOMBAT_FILENAME_SIZE];
	FILE *file_figures;

	bonus1fig = (wombatB1Figure *)malloc(sizeof(wombatB1Figure));

	if (scenario_name[0] != '\0')
	{
		strncpy(filename, SCENARIO_PATH, WOMBAT_FILENAME_SIZE);
		strncat(filename, "bonus1figures", WOMBAT_FILENAME_SIZE-strlen(filename));
		if (Bonus1ReadFiguresFromFile(filename) == 0)
		{
			wombatlog("Read figures for scenario", 2);
			return 0;
		}
		else {
			wombatlog("Problem loading figures for scenario, loading defaults", 2);
		}
	}
		

#ifdef DATADIR

	strncpy(filename, DATADIR, WOMBAT_FILENAME_SIZE);
	strncat(filename, "/", WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, PACKAGE, WOMBAT_FILENAME_SIZE-strlen(filename));
	strncat(filename, "/bonus1figures", WOMBAT_FILENAME_SIZE-strlen(filename));
	Bonus1ReadFiguresFromFile(filename);

#endif

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "bonus1figures", PACKAGE);
	Bonus1ReadFiguresFromFile(filename);

	return 0;
}

int Bonus1SaveFigures()
{
	int i, j, k;
	char filename[WOMBAT_FILENAME_SIZE];
	FILE *file_figures;

	configdefaultfilelocation(filename, WOMBAT_FILENAME_SIZE, "bonus1figures", PACKAGE);
	file_figures = fopen(filename, "w");
	if (file_figures == NULL)
	{
		wombatlog("Can't open file to save figures", 5);
		return 1;
	}

	for (i=0; i<getConfigInt("bonus1_fig_nr"); i++)
	{
		fprintf(file_figures, "%i\n", bonus1fig[i].wallsnumber);
		for(j=0; j<bonus1fig[i].wallsnumber; j++)
		{
			fprintf(file_figures, "%i\n", bonus1fig[i].wall[j].vertexnumber);
			for (k=0; k<bonus1fig[i].wall[j].vertexnumber; k++)
			{
				fprintf(file_figures, "%f/%f/%f\n", bonus1fig[i].wall[j].vertex[k].x, bonus1fig[i].wall[j].vertex[k].y, bonus1fig[i].wall[j].vertex[k].z);
			}
		}
	}

	fclose(file_figures);

	return 0;
}