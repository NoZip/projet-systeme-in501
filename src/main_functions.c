#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "main_functions.h"

/**
 * Créer une archive et y écris les fichiers.
 * @param archive le nom de l'archive ou seront écris les fichiers
 * @param file_name un tableau contenant les fichiers
 */
void create(char * archive, char * file_names[]){
	FILE * file;
	int size = 0;
	size = sizeof(file_names) / sizeof(char*) + 1 ;

	// Sélection de la sortie standard
	if (archive == NULL)
		file = stdout;

	// Ou ouvrerture du fichier tar
	else
		file = fopen(archive, "wb");
	assert(file);
	for(int i=0; i < size; i++){
		if(file_names[i][strlen(file_names[i]+1)] == '/'){

			// Répertoire
			write_directory(file, file_names [i]);
		}
		else
			// Fichier
			write_file(file, file_names[i]);
	}
	fclose(file);

}

/**
 * Extrait les fichiers de l'archive
 * @param archive l'archive contenant les fichiers
 */
void extract(char * archive){
	Header header;

	// Ouvrerture du fichier archive
	FILE * file = fopen(archive, "rb");
	assert(file);

	// Test fin du fichier
	while(!feof(file)){
		header = read_header(file);
		extract_file(file, header);
	}
	fclose(file);
}

/**
 * Liste les fichiers contenue dans une archive
 * @param archive l'archive contenant les fichiers
 */
void list(char * archive){
	FILE * file;
	Header header;

	// Ouverture du fichier archive
	file = fopen(archive, "rb");
	assert(file);

	// Test fin du fichier
	while(!feof(file)){
		header = read_header(file);

		if(header_type_flag(header) == 5)
			// Répertoire
			printf("%s",header_file_name(header));
		else{
			// Fichier
			printf("%s",header_file_name(header));
			fseek(file, header_file_size(header), SEEK_CUR);
		}
	}
	fclose(file);
}

/**
 * Liste les headers d'une archive
 * @param archive L'archive à lister
 * @return La liste des headers de l'archive
 */
Header * list_header(char * archive){

	Header * list = NULL;
	int i = 0;
	Header header;


	// Ouverture de l'archive
	FILE * file = fopen(archive, "r");
	assert(file);
	int fd = open(archive, O_RDONLY, 0666);
	off_t taille = lseek(fd, 0, SEEK_END);

	// Test fin du fichier
	header = read_header(file);
	while(ftell(file) == taille){
		// Réallocation du tableau

		list = (Header *) realloc(list, (i+1)*sizeof(Header));

		if(header_type_flag(header) == 5){
			list[i] = header; // Répertoire
		}
		else{
			list[i] = header; // Fichier
		}

		header = read_header(file);
		i++;
	}
	return list;
}

/**
 * Ajoute un fichier à une archive.
 * @param archive L'archive où ajouter le fichier.
 * @param file_names La liste des fichiers à ajouter.
 * @param update Si c'est une MAJ de l'archive ou non.
 */
void add(char * archive, char * file_names[], bool update){
	Header* list = NULL;
	Header header;
	FILE * file;
	int size_file_names = sizeof(file_names)/sizeof(char*);
	int size_list = sizeof(list)/sizeof(Header*);

	// Test de l'option -u
	if(update == false){
		list = list_header(archive);
		extract(archive);
		file = fopen(archive, "wb");
		assert(file);

		//ajout des anciens fichiers
		for(int i=0; i < size_list; i++){
			Header tmp = list[1];
			if(header_type_flag(tmp) == '5'){
				write_directory(file, header_linked_file_name(list[i])); // Répertoire
			}
			else{
				write_file(file, header_linked_file_name(list[i])); // Fichier
			}
		}

		// Ajout des nouveaux fichiers
		for(int i=0; i < size_file_names; i++){
			if(file_names[i][strlen(file_names[i])] == '/')
				write_directory(file, header_linked_file_name(list[i])); // Répertoire
			else
				write_file(file, header_linked_file_name(list[i])); // Fichier
		}
		fclose(file);
		free(list);
	}
	else{
	}


}
