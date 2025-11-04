#include <stdio.h>
#include <glib.h>
#include "album.h"
#include "../Utilities/utils.h"

//Estrutura que representa um álbum.
typedef struct album{
} Album;


//Função que liberta a memória associada a um álbum.
void free_album(Album* album) {
    if (album) {
        g_free(album);
    }
}

//Função que liberta a memória de um álbum, usado em operações de hash table.
void free_album_void(gpointer album){
    free_album(album);
}

Album* findAlbum(GHashTable* table, const char* id) {
    return (Album*) g_hash_table_lookup(table, id);
}

//Função que clona um álbum.
Album* cloneAlbum(const Album* album) {
    if (!album) return NULL;

    Album* new_album = malloc(sizeof(Album));
    if (!new_album) return NULL;

    return new_album;
}


//Função que constroi um álbum a partir de uma linha de dados.
Album* completa_Album(char* line, char** tokens) {
    const char *errors_filepath = "resultados/albums_errors.csv";

    if (tokens == NULL || tokens[0] == NULL || tokens[1] == NULL || tokens[3] == NULL) {
        FILE *file = fopen(errors_filepath, "a");
        if (!file) {
            perror("Erro ao abrir o ficheiro de erros");
            return NULL;
        }

        write_csv_header_if_new(file, "\"id\";\"title\";\"artist_id\";\"year\";\"producers\"");


        fprintf(file, "%s\n", line);

        fclose(file);
        return NULL;
    }

    Album* album = malloc(sizeof(Album));
    if (!album) return NULL;


    return album;
}
