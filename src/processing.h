#ifndef INCLUDE_PROCESSING_H
#define INCLUDE_PROCESSING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#include "header.h"

extern bool VERBOSE;

void write_directory(FILE * archive, char * directory_name);
void extract_directory(FILE * archive, Header directory_header);

void write_file(FILE * archive, char * file_name);
void extract_file(FILE * archive, Header file_header);

#endif
