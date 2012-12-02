#ifndef INCLUDE_PROCESSING_H
#define INCLUDE_PROCESSING_H

#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

#include "header.h"

bool VERBOSE = false;

void write_directory(FILE * archive, char * directory_name);
void list_directory(FILE * archive, Header directory_header);
void extract_directory(FILE * archive, Header directory_header);

void write_file(FILE * archive, char * file_name);
void list_file(FILE * archive, Header file_header);
void extract_file(FILE * archive, Header file_header);

int create(char * filename, char * file_names[]);
int list(char * filename);
int add(char * filename, char * file_names[], bool update);
int extract(char * filename);

#endif
