#include <stdlib.h>
#inlcude <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

#include "main_fonctions.h"

/**
 * Créer une archive et y écris les fichiers.
 * @param archive le nom de l'archive ou seront écris les fichiers
 * @param file_name un tableau contenant les fichiers
 */
void create(char * archive, char * file_names[]){
  FILE* file;
  int size;
  size = sizeof(file_names) / sizeof(char*);
  if (archive == NULL) file = stdout;
  else file = fopen(archive, "wb");
  assert(file);
  for(int i=0, i<size, i++)
      if(file_name[i][strlen(file_names[i])] == "/")
	write_directory(file, file_names [i]);
      else
	write_file(file, file_names[i]);
    fclose();
  }
}

/**
 * Extrait les fichiers de l'archive
 * @param archive l'archive contenant les fichiers
 */
void extract(char * archive){
  FILE* file;
  Header header;
  file = fopen(archive, "rb");
  assert(file);
  while(!feof(file)){
    header = read_header(file);
    extract_file(file, header);
  }
  close();
}

/**
 * Liste les fichiers contenue dans une archive
 * @param archive l'archive contenant les fichiers
 */
void list(char * archive){
  FILE* file;
  Header header;
  file = fopen(archive, "rb");
  assert(file);
  while(!feof(file)){
    header = read_header(file);
    if(header_type_flag(header == 5))
      fprintf(header_file_name(header));
    else{
      fprintf(header_file_name(header));
      fseek(file, header_file_size(header), SEEK_CUR);
    }
  }
  close();
}

/**
 *
 *
 */
void add(char * archive, char * file_names[], bool update){
  //
}
