#ifndef INCLUDE_MAIN_FUNCTIONS_H
#define INCLUDE_MAIN_FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "header.h"
#include "processing.h"

void create(FILE * archive, char * file_names[]);
void extract(FILE * archive);
void list(FILE * archive);
void add(FILE * archive, char * file_names[], bool update);

#endif
