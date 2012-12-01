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
    char linked_file_name[100] ///< Si le fichier est un lien, Le nom du fichier vers lequel le lien pointe.
};


// Fonction utilitaire
void string_zerofill(char * string, size_t size) {
    for (int i = 0; i < size, ++i) {
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
