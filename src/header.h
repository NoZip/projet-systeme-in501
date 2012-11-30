#ifndef INCLUDE_HEADER_H
#define INCLUDE_HEADER_H

#include <stdio.h>

#define REGULAR_FILE_TYPE '0'
#define HARD_LINK_TYPE '1'
#define SYMBOLIC_LINK '2'
#define CHARACTER_SPECIAL_TYPE '3'
#define BLOCK_SPECIAL_TYPE '4'
#define DIRECTORY_TYPE '5'

typedef struct Header Header;

void read_header(FILE * file, Header * header);
void write_header(FILE * file, Header  * header);

#endif
