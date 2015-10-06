#include "configfile.h"

/* Some functions here inspired by Extreme Tuxracer configuration files:
https://extremetuxracer.svn.sourceforge.net/ */

int configcreatedirforfile(char *path)
{
	char directory[CONF_PATH_SIZE];
	int i, lastslash;
	struct stat buf;

	if (strlen(path) > CONF_PATH_SIZE)
		return 1;
//s	directory = (char*)malloc(sizeof(char)*(strlen(path)+2));
//	strncpy = (directory, path, CONF_PATH_SIZE);

//	printf("Bede kopiowal\n");
	lastslash=1;
	for (i=strlen(path)-1; i>=0; i--)
	{
		printf("zaczynam: ");
		if (lastslash && path[i] == '/' && path[i-1] != '\\')
		{
			directory[i] = '\0';
			lastslash=0;
		}
		else
		{
			directory[i] = path[i];
		}
		printf("%i:%c; ", i, directory[i]);
	}
	printf("\n");

printf("%s\n", directory);

	if (strlen(directory) <= 1)
	{
//		printf("katalog glowny - cos jest zle!\n");
//		free (directory);
		return 0;
	}

	if (stat(directory, &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
		{
			if (access(directory, W_OK|R_OK) == 0)
			{
//				printf("katalog istnieje, stworz nastepny\n");
//				free(directory);
//				free(buf);
				return 0;
			}
			else
			{
//				printf("nie masz dostepu do katalogu: %i\n", errno);
//				free (directory);
//				free(buf);
				return 1;
			}
		}
		else
		{
//			printf("Na sciezce plik zamiast katalogu\n");
//			printf("Zwracam 1\n");
			return 1;
		}
	}

	if (errno == ENOENT)
	{
		if (configcreatedirforfile(directory) == 0)
		{
			if(mkdir(directory, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) == 0)
			{
//				printf("Stworzylem katalog %s\n", directory);
//				free(directory);
//				free(buf);
				return 0;
			}
			else
			{
//				printf("Jakis blad przy tworzeniu katalogu %s:%i\n", directory, errno);
//				free(directory);
//				free(buf);
		 		return 1;
			}
		}
	}

//	free(directory);
//	free(buf);
	return 1;
}

int configdefaultdir(char *dirname, int dirnamesize, char *packagename)
{
	struct passwd *pwent;

	pwent = getpwuid( getuid() );

	if (pwent == NULL)
	{
		return 1;
	}

	if (strlen(pwent->pw_dir) + strlen(packagename) + 3 > dirnamesize)
	{
		printf("Config directory name is too long - overflow!\n");
		return 1;
	}

	sprintf(dirname, "%s/.%s", pwent->pw_dir, packagename);

	return 0;
}

int configdefaultfilelocation(char *path, int pathsize, char *filename, char *packagename)
{
	char dirname[CONF_DEFAULTDIR_SIZE];

	if (configdefaultdir(dirname, CONF_DEFAULTDIR_SIZE, packagename))
	{
		return 1;
	}

	if (strlen(dirname) + strlen(filename) + 2 > pathsize)
	{
		printf("Config file name is too long - overflow!\n");
		return 1;
	}

	sprintf(path, "%s/%s", dirname, filename);

	return 0;
}

int configwrite(char *path, configentry *entries, int howmanyentries)
{
	FILE *configfile;
	int i, success;

	configfile = fopen(path, "w");
	if (configfile == NULL)
	{
		wombatlog("Can't open file to save configuration data", 5);
		printf("Cant open\n");
		return 1;
	}

	for (i=0; i<howmanyentries; i++)
	{
		success = 0;
		//printf("Type: %i\n", entries[i].type);
		if (strcspn(entries[i].name, " \t=") != strlen(entries[i].name))
		{
			printf("Niedozwolone znaki w nazwie: %s\n", entries[i].name);
			continue;
		}
		switch (entries[i].type) {
			case CONF_STRING:
			if (fprintf(configfile, "%s = \"%s\"\n", entries[i].name, entries[i].value.string) > 0)
				success=1;
			break;

			case CONF_INT:
			if (fprintf(configfile, "%s = %i\n", entries[i].name, entries[i].value.integer) > 0)
				success=1;
			break;

			case CONF_FLOAT:
			if (fprintf(configfile, "%s = %f\n", entries[i].name, entries[i].value.floatpoint) >0)
				success=1;
			break;

			case CONF_BOOL:
			if (entries[i].value.integer)
			{
				if (fprintf(configfile, "%s = true\n", entries[i].name) >0)
					success=1;
			}
			else
			{
				if (fprintf(configfile, "%s = false\n", entries[i].name) >0)
					success=1;
			}
			break;
		}

		if (success == 0)
			printf("Faile to write %s\n", entries[i].name);
			//wombatlog("Failed to write sthg to config", 5);
	}

	//printf("zamykam plik\n");
	fclose(configfile);

	//printf("zamknalem plik\n");

	return 0;
}

int configread(char *path, configentry *entries, int howmanyentries)
{
	FILE *configfile;
	int i, j;
	const int num = 2*(CONF_NAME_SIZE+CONF_STRING_SIZE);
	const int namesize = 2*CONF_NAME_SIZE;
	const int valuesize = 2*CONF_STRING_SIZE;
	char line[num], name[namesize], value[valuesize];

	configfile = fopen(path, "r");
	if (configfile == NULL)
	{
		//wombatlog("Can't open file to load configuration data", 5);
		printf("Cant open\n");
		return 1;
	}

	while (fgets(line, num, configfile) != NULL)
	{
		//printf("Line: %s|\n", line);
		if (strcspn(line, "#") != strlen(line))
		{
		//	printf("Znalazlem komentarz\n");
			if (line[strlen(line)-1] != '\n')
			{
				while (fgetc(configfile) != '\n')	// moving indicator to the end of the line
					;
		//		printf("Komentarz dluzszy niz do entera\n");
			}
			line[strcspn(line, "#")] = '\0';	// removing comment from the line
		}
		if (strlen(line) == 0)
		{
		//	printf("Linia zakomentowana\n");
			continue;
		}
		if (line[0] == '\n')
		{
			continue;
		}
		//printf("Line: %s|nr=%i\n", line, strcspn(line, "="));
		strncpy(name, line, strcspn(line, "="));	// copy name to "name" array
		if (strcspn(line, "=") < namesize)		// if found name
		{
			name[strcspn(line, "=")]='\0';
			//printf("First name: %s|\n", name);
			for (i=0; i < strlen(name); i++)		//removing blank characters
			{
				if (isblank(name[i]))
				{
					for (j=i; j<strlen(name); j++)
					{
						name[j] = name[j+1];
					}
					i--;
				}
			}		// name prepared

			strncpy(value, strchr(line, '=')+sizeof(char), valuesize);

			for (i=0; i < strlen(value); i++)		//removing blank characters
			{
				if (value[i] == '\n')
				{
					value[i] = '\0';
					break;
				}
				if (value[i] == '\"')			// dont remove blank characters between ""
				{
					while (i < strlen(value)-1 && value[i+1] != '\"')
						i++;
				}
				if (isblank(value[i]))
				{
					for (j=i; j<strlen(value); j++)
					{
						value[j] = value[j+1];
					}
					i--;
				}
			}		// name prepared
//			printf("%s|%c\n", value, value[0]);

			for (i=0; i < howmanyentries; i++)
			{
				if (strcmp(name, entries[i].name) == 0)
				{
					switch (entries[i].type) {
						case CONF_STRING:
							if (value[0] == '\"' && value[strlen(value)-2] == '\"')
							{
								if (strlen(value) > CONF_STRING_SIZE+sizeof(char))
								{
									strncpy(entries[i].value.string, value+sizeof(char), CONF_STRING_SIZE-1);
								}
								else
									strncpy(entries[i].value.string, value+sizeof(char), strlen(value)-3);
							}
							else
							{
								strncpy(entries[i].value.string, value, CONF_STRING_SIZE);
							}
							break;
						case CONF_INT:
							entries[i].value.integer = atoi(value);
							break;
						case CONF_FLOAT:
							entries[i].value.floatpoint = atof(value);
							break;
						case CONF_BOOL:
							if (strstr(value, "true") != NULL)
								entries[i].value.integer = 1;
							if (strstr(value, "false") != NULL)
								entries[i].value.integer = 0;
							break;
					}
					break;
				}
			}
		}
	}

	return 0;
}

int config_createentry_int(configentry *entry, char *name, int value)
{
	strncpy(entry->name, name, CONF_NAME_SIZE);
	entry->type = CONF_INT;
	entry->value.integer = value;

	return 0;
}

int config_createentry_float(configentry *entry, char *name, float value)
{
	strncpy(entry->name, name, CONF_NAME_SIZE);
	entry->type = CONF_FLOAT;
	entry->value.floatpoint = value;

	return 0;
}

int config_createentry_string(configentry *entry, char *name, char *value)
{
	strncpy(entry->name, name, CONF_NAME_SIZE);
	entry->type = CONF_STRING;
	strncpy(entry->value.string, value, CONF_STRING_SIZE);

	return 0;
}

int config_createentry_bool(configentry *entry, char *name, int value)
{
	strncpy(entry->name, name, CONF_NAME_SIZE);
	entry->type = CONF_BOOL;
	entry->value.integer = value;

	return 0;
}

int config_updateentry_int(configentry *entries, int nrofentries, char *name, int value)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_INT && strcmp(name, entries[i].name) == 0)
		{
			entries[i].value.integer = value;
			return 0;
		}
	}

	return 1;
}

int config_updateentry_string(configentry *entries, int nrofentries, char *name, char *value)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_STRING && strcmp(name, entries[i].name) == 0)
		{
			strncpy(entries[i].value.string, value, CONF_STRING_SIZE);
			return 0;
		}
	}

	return 1;
}

int config_updateentry_float(configentry *entries, int nrofentries, char *name, float value)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_FLOAT && strcmp(name, entries[i].name) == 0)
		{
			entries[i].value.floatpoint = value;
			return 0;
		}
	}

	return 1;
}

int config_updateentry_bool(configentry *entries, int nrofentries, char *name, int value)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_BOOL && strcmp(name, entries[i].name) == 0)
		{
			entries[i].value.integer = value;
			return 0;
		}
	}

	return 1;
}

int config_getentry_int(configentry *entries, int nrofentries, char *name)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_INT && strcmp(name, entries[i].name) == 0)
		{
			return entries[i].value.integer;
		}
	}

	return -1;
}

char *config_getentry_string(configentry *entries, int nrofentries, char *name)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_STRING && strcmp(name, entries[i].name) == 0)
		{
			return entries[i].value.string;
		}
	}

	return NULL;
}
float config_getentry_float(configentry *entries, int nrofentries, char *name)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_FLOAT && strcmp(name, entries[i].name) == 0)
		{
			return entries[i].value.floatpoint;
		}
	}

	return -1;
}
int config_getentry_bool(configentry *entries, int nrofentries, char *name)
{
	int i;

	for (i=0; i<nrofentries; i++)
	{
		if (entries[i].type == CONF_BOOL && strcmp(name, entries[i].name) == 0)
		{
			return entries[i].value.integer;
		}
	}

	return -1;
}
