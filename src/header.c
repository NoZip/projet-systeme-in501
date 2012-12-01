#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "header.h"

// Fonction utilitaire
void string_zerofill(char * string, size_t size) {
    for (int i = 0; i < size; ++i) {
        string[i] = '\0';
    }
}

/**
 * Initialise un header avec tous ces champs remplis de caractères nuls.
 * @param header Le header à initialiser.
 */
void init_header(Header * header) {
    string_zerofill(header->file_name, 100);
    string_zerofill(header->file_mode, 8);
    string_zerofill(header->owner_id, 8);
    string_zerofill(header->owner_group_id, 8);
    string_zerofill(header->file_size, 12);
    string_zerofill(header->last_modification, 12);
    string_zerofill(header->checksum, 8);
    string_zerofill(header->type_flag, 1);
    string_zerofill(header->linked_file_name, 100);
}

/// Le nombres d'octets vide dans le header.
const size_t BLOCK_TRIM = 512 - sizeof(Header);

/**
 * Récupère le contenu d'un header à partir d'un fichier.
 * @param file Le fichier à partir duquel est lu le header.
 * @param header Le header dans lequel écrire les données lues.
 */
void read_header(FILE * archive, Header * header) {
    assert(archive && header);

    fread(header->file_name, 1, 100, archive);
    fread(header->file_mode, 1, 8, archive);
    fread(header->owner_id, 1, 8, archive);
    fread(header->owner_group_id, 1, 8, archive);
    fread(header->file_size, 1, 12, archive);
    fread(header->last_modification, 1, 12, archive);
    fread(header->checksum, 1, 8, archive);
    fread(header->type_flag, 1, 1, archive);
    fread(header->linked_file_name, 1, 100, archive);

    // On saute les octets vides
    fseek(archive, BLOCK_TRIM, SEEK_CUR);
}

/**
 * Écrit le contenu d'un header dans un fichier.
 * @param file Le fichier dans lequel écrire.
 * @param header Le header à écrire dans le fichier.
 */
void write_header(FILE * archive, Header * header) {
    assert(archive && header);

    fwrite(header->file_name, 1, 100, archive);
    fwrite(header->file_mode, 1, 8, archive);
    fwrite(header->owner_id, 1, 8, archive);
    fwrite(header->owner_group_id, 1, 8, archive);
    fwrite(header->file_size, 1, 12, archive);
    fwrite(header->last_modification, 1, 12, archive);
    fwrite(header->checksum, 1, 8, archive);
    fwrite(header->type_flag, 1, 1, archive);
    fwrite(header->linked_file_name, 1, 100, archive);

    // Remplit les octets restants de caractères nuls.
    // TODO: Trouver une solution plus élégante
    char null_char = '\0';
    for (int i = 0; i < BLOCK_TRIM; ++i) {
        fwrite(&null_char, 1, 1, archive);
    }

    // Je sais pas si c'est utile, mais dans le doute (^-^)
    fflush(archive);
}
