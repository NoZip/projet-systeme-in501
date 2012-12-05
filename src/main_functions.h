#ifndef INCLUDE_MAIN_FUNCTIONS_H
#define INCLUDE_MAIN_FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "header.h"
#include "processing.h"

void create(char * archive, char * file_names[]);
void extract(char * archive);
void list(char * archive);
void add(char * archive, char * file_names[], bool update);

#endif
