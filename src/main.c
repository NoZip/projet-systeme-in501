#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "header.h"
#include "processing.h"
#include "main_functions.h"

int analyseOption(int argc, char* argv[]){

	char archive;
	bool c, r, u, x, t, z, f;
	int compt;
	int nb_param;
	char* filename= "";

	VERBOSE = false;
	while(1){
		compt = getopt(argc, argv, "hvc:tr:u:xf:zd:sparse");

		if (compt==-1){
			break;
		}

		switch(compt)
		{
		case 'h':
			printf("Y'a pas d'aide, débrouille toi sans.");
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
			f = true;
			break;
		case 'z':
			z = true;
			break;
		case 'd':
			/**suppr();*/
			break;
		}
	}

	nb_param=optind;
	if(f == true){
		archive = argv[optind];
		nb_param+=1;
	}

	filename = argv[nb_param];
	for(int i=nb_param+1; i<argc; i++){
		strcat(filename, argv[i]);
	}

	if(c == true)
		create(archive,filename);
	if(x == true)
		extract(archive);
	if(t == true)
		list(archive);
	if(z == true)
		return 1;
	if(d == true)
		return 1;
	if(r == true){
		if(u == true)
			add(archive, filename, true);
		else
			add(archive, filename, false);
	if(u == true)
		add(archive, filename, true);
	}
	return 1;
}


int main(int argc, char* argv[]){

	analyseOption(argc, argv);
	return 1;
}
