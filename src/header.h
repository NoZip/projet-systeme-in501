#ifndef INCLUDE_HEADER_H
#define INCLUDE_HEADER_H

#include <stdio.h>

#define REGULAR_FILE_TYPE '0'
#define HARD_LINK_TYPE '1'
#define SYMBOLIC_LINK '2'
#define CHARACTER_SPECIAL_TYPE '3'
#define BLOCK_SPECIAL_TYPE '4'
#define DIRECTORY_TYPE '5'

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

typedef struct Header Header;

void init_header(Header * header);

void read_header(FILE * file, Header * header);
void write_header(FILE * file, Header  * header);

#endif
