#include "processing.h"

#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

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
 */
void write_directory(FILE * archive, char * directory_name) {
    DIR * directory = opendir(directory_name);

    directory = opendir(directory_name);
    if (directory != NULL) {
        printf("Entrée dans le dossier %s\n", directory_name);

        // Écriture du header du dossier
        // TODO: ajouter des information supplémentaires au header.
        Header header;
        init_header(header);
        header.file_name = directory_name;
        header.type_flag = DIRECTORY_TYPE;

        printf("Écriture du header");
        write_header(archive, &header);

        // Analyse des entrées du dossier.
        struct dirent *entry;
        while (entry = readdir(directory)) {
            printf("Entrée %s du dossier %s\n", entry->d_name, directory_name);

            switch (entry->d_type) {
                // Fichier normal
                case DT_REG:
                    write_file(archive, entry->d_name);
                    break;

                // Dossier
                case DT_DIR:
                    write_directory(archive, entry->d_name);
                    break;
            }
        }
    }
    else {
        fprintf(stderr, "Impossible d'ouvrir le dossier %s.\n", directory_name);
        exit(1);
    }
}
