#ifndef INCLUDE_PROCESSING_H
#define INCLUDE_PROCESSING_H

#include <stdbool.h>

bool VERBOSE = false;

int create(char * filename, char * files[]);
int list(char * filename);
int add(char * filename, char * files[], bool update);
int extract(char * filename);

#endif
