#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

#ifndef CONFIGFILE_H
	#define CONFIGFILE_H

#define CONF_NAME_SIZE 40
#define CONF_STRING_SIZE 40
#define CONF_DEFAULTDIR_SIZE 40
#define CONF_PATH_SIZE 40

typedef enum {
	CONF_STRING,
	CONF_INT,
	CONF_FLOAT,
	CONF_BOOL
} configentry_type;

typedef union {
	char string[CONF_STRING_SIZE];
	int integer;
	float floatpoint;
} configentry_value;

typedef struct {
	char name[CONF_NAME_SIZE];
	configentry_type type;
	configentry_value value;
} configentry;

#endif

int configcreatedirforfile(char *path);
int configdefaultdir(char *dirname, int dirnamesize, char *packagename);
int config_createentry_float(configentry *entry, char *name, float value);
int configdefaultfilelocation(char *path, int pathsize, char *filename, char *packagename);
int configwrite(char *path, configentry *entries, int howmanyentries);
int configread(char *path, configentry *entries, int howmanyentries);
int config_createentry_int(configentry *entry, char *name, int value);
int config_createentry_float(configentry *entry, char *name, float value);
int config_createentry_string(configentry *entry, char *name, char *value);
int config_createentry_bool(configentry *entry, char *name, int value);
int config_updateentry_int(configentry *entries, int nrofentries, char *name, int value);
int config_updateentry_string(configentry *entries, int nrofentries, char *name, char *value);
int config_updateentry_float(configentry *entries, int nrofentries, char *name, float value);
int config_updateentry_bool(configentry *entries, int nrofentries, char *name, int value);
int config_getentry_int(configentry *entries, int nrofentries, char *name);
char *config_getentry_string(configentry *entries, int nrofentries, char *name);
float config_getentry_float(configentry *entries, int nrofentries, char *name);
int config_getentry_bool(configentry *entries, int nrofentries, char *name);
