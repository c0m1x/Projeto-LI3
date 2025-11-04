#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "album.h"
#include "parser.h"
#include "catalogoAlbum.h"
#include "utils.h"
#include "utils.h"
#include "catalogoArtist.h"



// Estrutura que representa um catálogo de álbuns utilizando uma tabela hash.
typedef struct album_catalog{
    GHashTable* albums;
} Album_catalog;


//Função que constroi o album a partir de uma linha de dados e insere-o no catálogo.
void buildAlbum(char* line, char** tokens, Album_catalog* catalogo, void* artist_catalogo){
    Album* album = completa_Album(line,tokens); 
    if(!album) return;
    char* album_id = g_strdup(tokens[0]);
    
    if(strlen(tokens[2]) < 15){ 
        char* artist_id = clean_artist_id(tokens[2]);
        add_num_album_artista((Artists_catalog *)artist_catalogo, artist_id);
        free(artist_id);
    }
    else {
    int count = 1; 
    for(int i = 0; tokens[2][i] != '\0'; i++) {
        if(tokens[2][i] == ',') count++;
    }
    
    char** array = process_string(tokens[2]);
    for(int i = 0; i < count; i++) {
        add_num_album_artista((Artists_catalog*)artist_catalogo, array[i]);
        free(array[i]); 
    }
    free(array); 
    }
    g_hash_table_insert(catalogo->albums,album_id, album);

}
//Função que constroi o album a partir de uma linha de dados e insere-o no catálogo.
void buildAlbum_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) {
    return buildAlbum(line, tokens, (Album_catalog *) structure1, structure2);
}

//Função que cria e inicializa um catálogo de álbuns a partir de um arquivo.
Album_catalog* albumToCatalog(char * path, void* structure1) {

    Album_catalog * catalogo = malloc(sizeof(Album_catalog));

    if(!catalogo) {
 
    }

    catalogo->albums = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_album_void);
    
    parser(path, buildAlbum_wrapper, catalogo, structure1, NULL, NULL, NULL); 
    if(catalogo==NULL){
    }

    return catalogo;
}

//Função que liberta a memória alocada para o catálogo de álbuns.
void free_album_catalog(Album_catalog* catalogo) {
    g_hash_table_destroy(catalogo -> albums);
    free(catalogo);
}

//Função que verifica se um álbum com o ID especificado está no catálogo.
gboolean album_Contains(Album_catalog* catalogo, char* album_id){
    if((catalogo == NULL) || album_id == NULL ) return 0;
    return g_hash_table_contains(catalogo->albums, album_id);
}
