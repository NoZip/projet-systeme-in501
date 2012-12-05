#ifndef INCLUDE_HEADER_H
#define INCLUDE_HEADER_H

#include <stdio.h>
#include <sys/types.h>

#define REGULAR_FILE_TYPE '0'
#define HARD_LINK_TYPE '1'
#define SYMBOLIC_LINK '2'
#define CHARACTER_SPECIAL_TYPE '3'
#define BLOCK_SPECIAL_TYPE '4'
#define DIRECTORY_TYPE '5'

typedef struct Header * Header;

Header init_header(void);

Header create_header(char * file_name,
                     mode_t file_mode,
                     // uid_t owner_id,
                     // gid_t owner_group_id,
                     off_t file_size,
                     time_t last_modification,
                     // char * checksum,
                     char type_flag,
                     char * linked_file_name);

void destruct_header(Header header);

char * header_file_name(Header header);
mode_t header_file_mode(Header header);
// uid_t * header_owner_id(Header header);
// gid_t * header_owner_groud_id(Header header);
off_t header_file_size(Header header);
time_t header_last_modification(Header header);
// char * header_checksum(Header header);
char header_type_flag(Header header);
char * header_linked_file_name(Header header);

Header read_header(FILE * file);
void write_header(FILE * file, Header  header);

#endif
