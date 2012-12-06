#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "header.h"
#include "processing.h"
#include "main_functions.h"

void print_help(){
	printf("Le programme \"tartiflette\" permet le stockage de different fichier ou dossier dans une seule et unique archive.\n");
	printf("\n");
	printf("Options possible:\n");
	printf("	-h  Afficher la présente page d'aide.\n");
	printf("	-v  Activer le mode verbose.\n");
	printf("	-c  Créer une nouvelle archive\n");
	printf("	-t  Lister le contenu d'une archive existante.\n");
	printf("	-r  Rajouter des fichiers ou dossier à une archive existante.\n");
	printf("	-u  Ajouter les fichiers plus récents que leurs copie déjà présente dans une archive existante.\n");
	printf("	-x  Extraitre une archive existante.\n");
	printf("	-f  Spécifier le nom de l'archive à utiliser.\n");
	printf("	-z  Filtrer l'archive au travers de gzip.\n");
	printf("	-d  Supprimer une archive existante.\n");
	printf("\n");
}

int analyseOption(int argc, char* argv[]){

	char * archive = NULL;
	bool c = false , r = false, u = false, x = false, t = false, z = false;
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
			c = true;
			break;
		case 't':
			t = true;
			break;
		case 'r':
			r = true;
			break;
		case 'u':
			u = true;
			break;
		case 'x':
			x = true;
			break;
		case 'f':
			archive = optarg;
			break;
		case 'z':
			z = true;
			break;
		case 'd':
			/**suppr();*/
			break;
		}
	}

	filename = malloc(sizeof(char*)*(argc-optind));

	if(x == true)
		extract(archive);
	if(t == true)
		list(archive);

	filename[0] = argv[optind];
	for(int i=optind+1; i<argc; i++){
		strcat(filename[i-optind], argv[i]);
	}
	if(c == true)
		create(archive,filename);

	if(r == true){
		add(archive, filename, u);
		if(u == true)
			add(archive, filename, u);
		if(z == true)
			return 1;
	}
	return 1;
}


int main(int argc, char* argv[]){
	analyseOption(argc, argv);
	return EXIT_SUCCESS;
}
