#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "header.h"
#include "processing.h"
#include "main_functions.h"

void print_help(){
	printf("Le programme \"tartiflette\" permet le stockage de different fichier ou dossier dans une seule et unique archive.\n");
	printf("Usage : ./tartiflette [-options] [Nom du fichier à créé si option f] [nom des fichiers à inclure dans l'archive]\n");
	printf("\n");
	printf("Options possible:\n");
	printf("	-h  Afficher la présente page d'aide.\n");
	printf("	-v  Activer le mode verbose.\n");
	printf("	-c  Créer une nouvelle archive\n");
	printf("	-t  Lister le contenu d'une archive existante.\n");
	printf("	-r  Rajouter des fichiers ou dossier à une archive existante.\n");
	printf("	-u  Ajouter les fichiers plus récents que leurs copie déjà présente dans une archive existante.\n");
	printf("	-x  Extraitre une archive existante.\n");
	printf("	-f  Spécifier le nom de l'archive à utiliser, doit être la dernière option appelée.\n");
	printf("	-z  Filtrer l'archive au travers de gzip.\n");
	printf("	-d  Supprimer une archive existante.\n");
	printf("\n");
}

int analyseOption(int argc, char* argv[]){

	char * archive = NULL;
	bool o_create = false , o_add = false, o_maj = false, o_extract = false, o_list = false, o_zip = false;
	int compt;
	char** filename = NULL;
	VERBOSE = false;

	while(1){
		compt = getopt(argc, argv, "hvctruxf:zd");

		if (compt==-1){
			break;
		}

		switch(compt)
		{
		case 'h':
			print_help();
			return 1;
			break;
		case 'v':
			VERBOSE = true;
			break;
		case 'c':
			o_create = true;
			break;
		case 't':
			o_list = true;
			break;
		case 'r':
			o_add = true;
			break;
		case 'u':
			o_maj = true;
			break;
		case 'x':
			o_extract = true;
			break;
		case 'f':
			archive = optarg;
			break;
		case 'z':
			o_zip = true;
			break;
		case 'd':
			/**suppr();*/
			break;
		}
	}

	filename = malloc(sizeof(char*)*(argc-optind));

	if(o_extract)
		extract(archive);
	if(o_list)
		list(archive);

	filename[0] = argv[optind];

	for(int i=optind+1; i<argc; i++){
		filename[i-optind]= argv[i];
	}

	if(o_create){
		create(archive,filename);
	}

	if(o_add){
		add(archive, filename, o_maj);
	}
	if(o_zip == true)
		execlp("gzip", "gzip", archive, NULL);

	return 1;
}


int main(int argc, char* argv[]){
	analyseOption(argc, argv);
	return EXIT_SUCCESS;
}
