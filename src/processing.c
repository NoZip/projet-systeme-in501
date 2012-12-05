#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "processing.h"

bool VERBOSE = false;

/**
 * Écrit le contenu d'un dossier (récursivement) fdans l'archive.
 * Écrit d'abord le header du dossier lui même, pui les headers et le
 * contenu de tous ces composants. Les sous-dossiers sont analysés
 * récursivement.
 *
 * @param archive L'archive dans laquelle on doit écrire.
 * @param directory_name Le chenim vers le dossier.
 *
 * @TODO peut être trouver un moyen d'éviter les appels récursifs.
 * @TODO trouver un moyen d'utiliser DT_REG et DT_DIR au lieu de leurs
 *       valeurs
 */
void write_directory(FILE * archive, char * directory_name) {
    assert(archive);

    // On récupère les attributs du dossier.
    struct stat directory_stats;
    if (stat(directory_name, &directory_stats) != 0) {
        fprintf(stderr, "Le dossier %s n'existe pas.\n", directory_name);
        return;
    }

    DIR * directory = opendir(directory_name);

    // Ouverture du répertoire
    directory = opendir(directory_name);

    // Traitement des érreurs d'ouverture
    if (directory == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le dossier %s.\n", directory_name);
        exit(1);
    }

    if (VERBOSE) printf("Entrée dans le dossier %s\n", directory_name);

    // Écriture du header du dossier
    // TODO: ajouter des information supplémentaires au header.
    Header header = create_header(directory_name,
                                  directory_stats.st_mode,
                                  directory_stats.st_size,
                                  directory_stats.st_mtime,
                                  DIRECTORY_TYPE,
                                  NULL);

    if (VERBOSE) printf("Écriture du header");

    //Écriture du Header
    write_header(archive, header);

    // Analyse des entrées du dossier.
    struct dirent *entry;
    while ((entry = readdir(directory))) {
        // On ne traite pas les répertoires spéciaux pour éviter les
        // boucles infinies.
        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
            if (VERBOSE) printf("Entrée %s du dossier %s\n", entry->d_name, directory_name);

            // Construction du nom complet du fichier
            char path[100] = "";
            strcpy(path, directory_name);
            strcat(path, entry->d_name);

            switch(entry->d_type) {
                case 8: //DT_REG
                    // Fichier normal
                    write_file(archive, path);
                    break;

                case 4: //DT_DIR
                    // Dossier
                    strcat(path, "/");
                    write_directory(archive, path);
                    break;
            }
        }
    }

    if (VERBOSE) printf("Fin du traitement du dossier %s\n", directory_name);
}

/**
 * Extrait un dossier de l'archive.
 * Cette fonction n'extraiera que les données du dossier, et pas des
 * autres dossiers ou fichiers qu'il contient.
 *
 * @param archive L'archive de laquelle le dossier sera extrait.
 * @param directory_header Le header du dossier à extraire.
 *
 * @TODO ajouter l'écriture des droits du dossier.
 */
void extract_directory(FILE * archive, Header directory_header) {
    char * directory_name = header_file_name(directory_header);
    mode_t directory_mode = header_file_mode(directory_header);

    // On crée le dossier
    mkdir(directory_name, directory_mode);
}

/**
 * Écrit un fichier dans l'archive.
 *
 * @param archive L'archive dans laquelle sera écrite le fichier.
 * @param file_name le nom (chemin) du fichier.
 * @param verbose définit la verbosité de la sortie standard.
 */
void write_file(FILE * archive, char * file_name) {
    assert(archive);

    // On récupère les attributs du fichier.
    struct stat file_stats;
    if (stat(file_name, &file_stats) != 0) {
        fprintf(stderr, "Le fichier %s n'existe pas.\n", file_name);
        return;
    }

    if (VERBOSE) printf("Écriture du fichier %s\n", file_name);

    Header file_header = create_header(file_name,
                                       file_stats.st_mode,
                                       file_stats.st_size,
                                       file_stats.st_mtime,
                                       REGULAR_FILE_TYPE,
                                       NULL);

    if (VERBOSE) printf("Écriture du header\n");

    // Écriture du header
    write_header(archive, file_header);

    // Ouverture du fichier source
    FILE * input_file = fopen(file_name, "rb");
    assert(input_file);

    if (VERBOSE) printf("Écriture des données du fichier (%ldo).\n", file_stats.st_size);

    // Écriture des données dans le fichier
    long int bytes_written = 0;
    char buffer[1024];
    while (!feof(input_file)) {

        // On lit les données dans le fichier source.
        int n = fread(&buffer, sizeof(char), 1024, input_file);

        // On écrit dans l'archive en vérifiant si la quantité de
        // données lus et écrites sont les mêmes
        assert(n == fwrite(&buffer, sizeof(char), n, archive));

        bytes_written += n;
        if (VERBOSE) printf("%ld octets écrits\n", bytes_written);
    }

    if (VERBOSE) printf("Fin de l'écriture du fichier %s\n", file_name);

    fflush(archive);
}

/**
 * Extrait un fichier de l'archive.
 * Il aura le même nom nom et la même position dans l'arborescence
 * que décrit dans son header. Il est important de noter que pour le bon
 * fonctionnement de cette fonction, le curseur de l'archive doit être
 * situé juste après le header qui décrit le fichier. Ce qui normalement
 * le cas après l'éxécution de read_header.
 *
 * @param archive l'archive de laquelle extraire le fichier.
 * @param file_header Le header correspondant au fichier à extraire.
 */
void extract_file(FILE * archive, Header file_header) {
    assert(archive);

    char * file_name = header_file_name(file_header);
    off_t file_size = header_file_size(file_header);

    // Ouverture du fichier de sortie
    FILE * output_file = fopen(file_name, "wb");
    assert(output_file);

    if (VERBOSE) printf("Extraction du fichier %s (%ldo)\n", file_name, file_size);

    // Écriture dans le fichier
    long int bytes_written = 0;
    char buffer[1024];
    while(bytes_written < file_size) {
        // Lecture des données de l'archive
        int n = fread(&buffer, sizeof(char), 1024, archive);

        // Écriture des données dans le fichier destination
        // En vérifiant que les données lues et écrites sont identiques
        assert(n == fwrite(&buffer, sizeof(char), n, output_file));

        bytes_written += n;
        if (VERBOSE) printf("%ld octets écrits\n", bytes_written);
    }

    fclose(output_file);

    // Écriture des droits du fichier
    chmod(file_name, header_file_mode(file_header));

    // Écritures de la date de dernière modification dans le fichier
    struct utimbuf file_times = {time(NULL), header_last_modification(file_header)};
    utime(file_name, &file_times);

    if (VERBOSE) printf("Extraction de %s terminée.\n", file_name);

    fflush(output_file);
}
