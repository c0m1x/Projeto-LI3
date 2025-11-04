#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "artists.h"
#include "parser.h"
#include "catalogoArtist.h"

//Estrutura que representa um catálogo de artistas utilizando uma tabela hash.
typedef struct artists_catalog{
    GHashTable* artists;
}Artists_catalog;

//Obtém um artista do catálogo pelo seu ID, retornando uma cópia do artista encontrado.
Artist* get_artist(Artists_catalog* catalogo, char* id){
    if((catalogo == NULL) || id == NULL ) return NULL;
    Artist* artista = g_hash_table_lookup(catalogo->artists, id);
    if(!artista) return NULL;

    return cloneArtista(artista);
}

// Obtem o valor da duracao total de um artista
void add_duracao_artist_catalog(Artists_catalog* catalogo, char* id, int duracao){
    Artist* artista = g_hash_table_lookup(catalogo->artists, id);
    if(artista){
        add_duracao_artist(artista, duracao);      
    }   
    else{
        printf("Artista não encontrado para ID: %s\n", id);
    }

}

// Obtem o profit total de um artista
void add_profit_artist_catalog(Artists_catalog* catalogo, char* id){
    Artist* artista = g_hash_table_lookup(catalogo->artists, id);
    if(artista){
        char* type = get_artist_type(artista);
        double recipe = get_artist_recipe_per_stream(artista);

        if(strcmp(type,"individual")==0){
            add_profit_artist(artista, recipe);
        }
        else{
            add_profit_artist(artista, recipe);
            const GArray* artists_ids = get_artist_id_constituent(artista);

            for(int i = 0; i < artists_ids->len; i++){
                char* current = g_array_index(artists_ids, char*, i);               
                Artist* current_artist = g_hash_table_lookup(catalogo->artists, current);
                add_profit_artist(current_artist, (recipe/artists_ids->len));
            }           
        }   
        free(type);  
    }
    else{
        printf("Artista não encontrado para ID: %s\n", id);
    }   
}

// Obtem o numero de albuns de um artista
void add_num_album_artista(Artists_catalog* catalogo, char* id){
    Artist* artista = g_hash_table_lookup(catalogo->artists, id);
    if(artista){
        add_num_albuns_individual(artista);      
    }
    else{
        printf("Artista não encontrado para ID: %s\n", id);
    }   
}

//Obtém a tabela hash de artistas do catálogo.
GHashTable* get_artists_table(Artists_catalog* catalog){
    return catalog->artists;
}

//Verifica se um artista está contido no catálogo.
gboolean artist_Contains(Artists_catalog* catalogo, char* id){
    if((catalogo == NULL) || id == NULL ) return 0;
    return g_hash_table_contains(catalogo->artists, id);
}


void buildArtist(char* line, char** tokens, Artists_catalog* catalogo){
    Artist* artist = completa_Artist(line,tokens); 
    if(!artist) return;
    char* artist_id = g_strdup(tokens[0]);

    g_hash_table_insert(catalogo->artists,artist_id, artist);

}

//Função que constroi o artista a partir de uma linha de dados e insere-o no catálogo.
void buildArtist_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) {
    return buildArtist(line, tokens, (Artists_catalog *) structure1);
}

//Função que cria e inicializa um catálogo de artistas a partir de um arquivo.
Artists_catalog* artistsToCatalog(char * path) {

    Artists_catalog * catalogo = malloc(sizeof(Artists_catalog));

    if(!catalogo) {
 
    }

    catalogo->artists = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_artist_void);
    
    parser(path, buildArtist_wrapper, catalogo, NULL, NULL, NULL, NULL); 
    if(catalogo==NULL){
    }

    return catalogo;
}

//Liberta a memória associada ao catálogo de artistas.
void free_artists_catalog(Artists_catalog* catalogo) {
    g_hash_table_destroy(catalogo -> artists);
    free(catalogo);
}



//Destrói a hash table de artistas do catálogo.
void artist_hash_table_destroy(Artists_catalog* catalog){
    if(catalog){
        g_hash_table_destroy(catalog->artists);
        free(catalog);
    }
}


