#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "header.h"

/**
 * Représente un header tar POSIX.
 * Un header POSIX fait une taille de 512 octets, les octets non
 * utilisés étant remplis par des caractères nuls (\0).
 * Les champs sont des chaines de caractères ASCII finissant par un
 * caractère nul. Les valeurs numériques (file_size par exemple)
 * sont écrits en octal. Ainsi 1024 sera écrit: 2000.
 * @note Pour l'instant, on est pas obligé de traiter tous les champs.
 */
struct Header {
    char file_name[100]; ///< Le nom de fichier.
    char file_mode[8]; ///< Les droits du fichier.
    char owner_id[8]; ///< L'id de l'utilisateur propriétaire.
    char owner_group_id[8]; ///< L'id du groupe propriétaire.
    char file_size[12]; ///< La taille du fichier (en octets).
    char last_modification[12]; ///< Le timestamp UNIX de dernière modification
    char checksum[8]; ///< La somme de contrôle du fichier.
    char type_flag[1]; ///< Un drapeau permettant de déterminer le type du fichier.
    char linked_file_name[100] ///< Si le fichier est un lien, Le nom du fichier vers lequel le lien pointe.
};


size_t BLOCK_TRIM = 512 - sizeof(Header);

/**
 * Récupère le contenu d'un header à partir d'un fichier.
 * @param file Le fichier à partir duquel est lu le header.
 * @param header Le header dans lequel écrire les données lues.
 */
void read_header(FILE * file, Header * header) {
    assert(file && header);

    fread(header->file_name, 1, 100, file);
    fread(header->file_mode, 1, 8, file);
    fread(header->owner_id, 1, 8, file);
    fread(header->owner_group_id, 1, 8, file);
    fread(header->file_size, 1, 12, file);
    fread(header->last_modification, 1, 12, file);
    fread(header->checksum, 1, 8, file);
    fread(header->type_flag, 1, 1, file);
    fread(header->linked_file_name, 1, 100, file);

    // On saute les octets vides
    fseek(file, BLOCK_TRIM, SEEK_CUR);
}

/**
 * Écrit le contenu d'un header dans un fichier.
 * @param file Le fichier dans lequel écrire.
 * @param header Le header à écrire dans le fichier.
 * @TODO Remplir l'espace restant de caractères nuls (\0)
 */
void write_header(FILE * file, Header * header) {
    assert(file && header);

    fwrite(header->file_name, 1, 100, file);
    fwrite(header->file_mode, 1, 8, file);
    fwrite(header->owner_id, 1, 8, file);
    fwrite(header->owner_group_id, 1, 8, file);
    fwrite(header->file_size, 1, 12, file);
    fwrite(header->last_modification, 1, 12, file);
    fwrite(header->checksum, 1, 8, file);
    fwrite(header->type_flag, 1, 1, file);
    fwrite(header->linked_file_name, 1, 100, file);
}
