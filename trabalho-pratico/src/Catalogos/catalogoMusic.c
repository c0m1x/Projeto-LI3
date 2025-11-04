#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "catalogoMusic.h"
#include "artists.h"

// Estrutura que representa um catálogo de músicas utilizando uma tabela hash.
typedef struct musics_catalog {
    GHashTable *musics;
} Musics_catalog;

// Obtém uma música do catálogo pelo seu ID, retornando uma cópia da música encontrada.
Music* getMusic(Musics_catalog* catalogo, char* id) {
    if ((catalogo == NULL || id == NULL)) return NULL; 
    Music* musica = g_hash_table_lookup(catalogo->musics, id);
    if (!musica) return NULL;
    return cloneMusica(musica);
}

// Obtém o genero de uma música pelo ID no catálogo.
char* getMusic_genre(Musics_catalog* catalogo, char* music_id){
    if(catalogo == NULL || music_id == NULL) return NULL;
    Music* musica = g_hash_table_lookup(catalogo->musics, music_id);
    if(!musica) return NULL;
    return get_music_genre(musica);
}

const GArray* get_artist_music_catalog(Musics_catalog* catalogo, char* music_id){
    if ((catalogo == NULL || music_id == NULL)) return NULL; 
    Music* musica = g_hash_table_lookup(catalogo->musics, music_id);
    if (!musica) return NULL;
    return get_music_artist_idss(musica);
}

// Verifica se uma música existe no catálogo.
gboolean music_exists(Musics_catalog* catalog, const char* music_id) {
    if (catalog == NULL || (catalog->musics) == NULL) {
        
        return FALSE;  
    }
    return g_hash_table_contains(catalog->musics, music_id);
}


// Constrói e adiciona uma música ao catálogo a partir de tokens.
void buildMusic(char* line, char** tokens, Musics_catalog* catalogo, void* structure1, void* structure2) {
    
    Music* musica = completa_Musica(line, tokens, structure1,structure2);
    if (!musica) return;

    char* music_id = g_strdup(tokens[0]);
  
    g_hash_table_insert(catalogo->musics, music_id, musica);

    
    const GArray* artists_ids = get_music_artist_idss(musica);

    for (int i = 0; i < artists_ids->len; i++) {
        char* artist_id = g_array_index(artists_ids, char*, i);
        int duracao = get_music_duration_int(musica);

        if (artist_id != NULL) {
            
            add_duracao_artist_catalog((Artists_catalog*)structure1, artist_id, duracao);
        } else {
            return;
        }
    }

}


// Wrapper para uso com a função parser, chamando buildMusic.
void buildMusic_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) {
    return buildMusic(line,tokens, (Musics_catalog*) structure1, structure2, structure3);
}

// Cria e carrega um catálogo de músicas a partir de um arquivo CSV.
Musics_catalog* musicsToCatalog(char* path, void* structure1, void* structure2) {
    Musics_catalog* catalogo = malloc(sizeof(Musics_catalog));
    catalogo->musics = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_music_void);
    parser(path, buildMusic_wrapper, catalogo, structure1, structure2, NULL, NULL);

    return catalogo;
}

// Liberta a memória alocada para o catálogo de músicas.
void free_musics_catalog(Musics_catalog* catalogo) {
    g_hash_table_destroy(catalogo->musics);
    free(catalogo);
}

// Obtém o tamanho do catálogo de músicas.
int getMusicsLenght(Musics_catalog * catalogo) {
    return g_hash_table_size(catalogo->musics);
}

// Obtém a hash table de músicas do catálogo.
GHashTable* get_musics_hash_table(Musics_catalog* catalog) {
    if (catalog) {
        return catalog->musics;
    }
    return NULL; 
}

// Destrói a hash table de músicas do catálogo.
void music_hash_table_destroy(Musics_catalog* catalog){
    if(catalog){
        g_hash_table_destroy(catalog->musics);
        free(catalog);
    }
}

