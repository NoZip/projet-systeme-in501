#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

#include "header.h"

/**
 * Représente un header tar POSIX.
 * Un header POSIX fait une taille de 512 octets, les octets non
 * utilisés étant remplis par des caractères nuls (\0).
 * Les champs sont des chaines de caractères ASCII finissant par un
 * caractère nul. Les valeurs numériques (file_size par exemple)
 * sont écrits en octal. Ainsi 1024 sera écrit: 2000. Les noms des
 * entrées sont ecrits selon la structure suivante:
 *  - "dossier_racine/sous_dossier/.../dossier/" pour les dossiers.
 *  - "dossier_racine/sous_dossier/.../dossier/fichier" pour un fichier.
 *
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
    char linked_file_name[100]; ///< Si le fichier est un lien, Le nom du fichier vers lequel le lien pointe.
};

/**
 * crée un header avec tous ces champs remplis de caractères nuls.
 * @return Le header initialisé.
 */
Header init_header(void) {
    Header header = (Header) malloc(sizeof(struct Header));
    assert(header);

    memset(header->file_name, '\0', 100);
    memset(header->file_mode, '0', 8);
    memset(header->owner_id, '0', 8);
    memset(header->owner_group_id, '0', 8);
    memset(header->file_size, '0', 12);
    memset(header->last_modification, '0', 12);
    memset(header->checksum, '\0', 8);
    memset(header->type_flag, '\0', 1);
    memset(header->linked_file_name, '\0', 100);

    return header;
}

/**
 * Crée un header à partir de valeur pour ses champs.
 * @param file_name Le nom du fichier/dossier que décrit le header.
 * @param file_size La taille du fichier que décrit le header.
 * @param last_modification La date de dernière modification du fichier
 *        (Timestamp UNIX) du fichier que décrit le header.
 * @param type_flag Le type du fichier que décrit le header.
 * @param linked_file_name Le nom du fichier vers lequel pointe le lien
 *        de ce fichier
 * @return Un header initialisé avec les valeurs données en entrée.
 */
Header create_header(char * file_name,
                     mode_t file_mode,
                     // uid_t owner_id,
                     // gid_t owner_group_id,
                     off_t file_size,
                     time_t last_modification,
                     // char * checksum,
                     char type_flag,
                     char * linked_file_name) {
    Header header = init_header();

    if (file_name) strcpy(header->file_name, file_name);
    if (file_mode) sprintf(header->file_mode, "%o", file_mode);
    // if (owner_id) sprintf(header->owner_id, "%lo", owner_id);
    // if (owner_group_id) sprintf(header->owner_group_id, "%lo", owner_group_id);
    if (file_size) sprintf(header->file_size, "%lo", file_size);
    if (last_modification) sprintf(header->last_modification, "%lo", last_modification);
    // if (checksum != NULL) strcpy(header->checksum, checksum);
    if (type_flag) *header->type_flag = type_flag;
    if (linked_file_name) strcpy(header->linked_file_name, linked_file_name);

    return header;
}

void destruct_header(Header header) {
    free(header);
}

char * header_file_name(Header header) {
    return header->file_name;
}

mode_t header_file_mode(Header header) {
    mode_t file_mode;
    sscanf(header->file_mode, "%o", &file_mode);
    return file_mode;
}
// uid_t * header_owner_id(Header header);
// gid_t * header_owner_groud_id(Header header);

off_t header_file_size(Header header) {
    off_t file_size;
    sscanf(header->file_size, "%lo", &file_size);
    return file_size;
}

time_t header_last_modification(Header header) {
    time_t last_modification;
    sscanf(header->last_modification, "%lo", &last_modification);
    return last_modification;
}

// char * header_checksum(Header header);

char header_type_flag(Header header) {
    return *header->type_flag;
}

char * header_linked_file_name(Header header) {
    return header->linked_file_name;
}

/// Le nombres d'octets vide dans le header.
const size_t BLOCK_TRIM = 512 - sizeof(Header);

/**
 * Récupère le contenu d'un header à partir d'un fichier.
 * @param file Le fichier à partir duquel est lu le header.
 * @return Le header dans lequel ont été écrites les données lues.
 */
Header read_header(FILE * archive) {
    assert(archive);

    Header header = init_header();

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

    return header;
}

/**
 * Écrit le contenu d'un header dans un fichier.
 * @param file Le fichier dans lequel écrire.
 * @param header Le header à écrire dans le fichier.
 */
void write_header(FILE * archive, Header header) {
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
    char trim[BLOCK_TRIM];
    memset(trim, '\0', BLOCK_TRIM);
    fwrite(&trim, sizeof(char), BLOCK_TRIM, archive);

    // Je sais pas si c'est utile, mais dans le doute (^-^)
    fflush(archive);
}
