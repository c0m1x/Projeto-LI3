#include "artists.h"
#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "artists_validation.h"
#include "utils.h"
#include "music_validation.h"

//VER DEPOIS
#include "artists.h"
#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "artists_validation.h"
#include "utils.h"
#include "music_validation.h"


// Estrutura que representa um artista.
typedef struct artist {      
    char* name;                      
    double recipe_per_stream;          
    GArray* id_constituent;    
    char* country;            
    char* type; 
    int duracaoTotal;
    int num_albuns_individual; 
    float profitTotal;             
} Artist;

// Função que devolve o numero de albuns individuais de um artista
int get_artist_num_albums(const Artist* artist){
    return artist->num_albuns_individual;
}

// Função que devolve o profit total de um artista
double get_artist_profitTotal(const Artist* artist){
    return artist->profitTotal;
}

// Função que devolve o nome de um artista
char* get_artist_name(const Artist* artist) {
    return remover_aspas_dup(artist->name);
}


// Função que devolve o pais de um artista
char* get_artist_country(const Artist* artist) {
    return strdup(artist->country);
}

// Função que devolve o tipo de um artista
char* get_artist_type(const Artist* artist) {
    return strdup(artist->type);
}
// Função que devolve o ID do constituinte de um artista
const GArray* get_artist_id_constituent(const Artist* artist) {

    return artist->id_constituent;
}

// Função que devolve o valor do "receita por stream" de um artista
double get_artist_recipe_per_stream(const Artist* artist) {
    return artist->recipe_per_stream;
}

// Função que devolve o valor da duracao total de um artista
int get_artist_duracaoTotal(const Artist* artist) {
    return artist->duracaoTotal;
}

// Função que adiciona uma duracao ao artista
void add_duracao_artist(Artist* artist, int duracao){

        artist->duracaoTotal += duracao;

}

// Função que adiciona um album ao artista
void add_num_albuns_individual(Artist* artist){
    artist->num_albuns_individual+=1;
}

// Função que adiciona profit ao ao artista
void add_profit_artist(Artist* artist, double recipe){
        artist->profitTotal += recipe;
 
}


// Função que liberta a memória associada a um artista
void free_artist(Artist* artist) {
    if (artist) {
        g_free(artist->name);
        g_free(artist->country);
        g_free(artist->type);
        for (guint i = 0; i < artist->id_constituent->len; i++) {
            g_free(g_array_index(artist->id_constituent, char*, i));
        }
        g_array_free(artist->id_constituent, TRUE);

        free(artist);
    }
}

// Função que liberta a memória de um artista, usado em operações de hash table
void free_artist_void(gpointer artist) {
    free_artist(artist);
}

// Função que procura um artista na tabela hash pelo ID
Artist* find_artist(GHashTable* table, const char* id) {
    return (Artist*) g_hash_table_lookup(table, id);
}

// Função que clona um artista
Artist* cloneArtista(Artist* artist){


    Artist* new_artist = malloc(sizeof(Artist));

    new_artist->name = g_strdup(artist->name);
    new_artist->recipe_per_stream = artist->recipe_per_stream;
    new_artist->id_constituent = g_array_sized_new(FALSE, FALSE, sizeof(char*), artist->id_constituent->len);
    new_artist->country = g_strdup(artist->country);
    new_artist->type = remover_aspas_dup(artist->type);
    new_artist->duracaoTotal = artist->duracaoTotal;
    
    for (guint i = 0; i < artist->id_constituent->len; i++) {
        char* artist_id = g_strdup(g_array_index(artist->id_constituent, char*, i));
        g_array_append_val(new_artist->id_constituent, artist_id);
    }
    new_artist->num_albuns_individual = artist->num_albuns_individual;
    new_artist->duracaoTotal = artist->duracaoTotal;
    new_artist->profitTotal = artist->profitTotal;

    return new_artist;
}


//  Função que valida os tokens de um artista
int valida_tokens_artistas(char** tokens) {
    char* constituent = remover_aspas_dup(tokens[4]);
    char* type = remover_aspas_dup(tokens[6]);

    int is_valid = is_artist_valid(constituent, type);

    free(constituent);
    free(type);

    if (is_valid==0) return 1;
    else return 0;
}


//Função que constroi um artista a partir de uma linha de dados e insere-o no catálogo
Artist* completa_Artist(char* line,char** tokens) {

    const char *errors_filepath = "resultados/artists_errors.csv";


    if (valida_tokens_artistas(tokens)==1) {

        FILE *file = fopen(errors_filepath, "a");
        if (!file) {
            perror("Erro ao abrir o ficheiro de erros");
            return NULL;
        }

        write_csv_header_if_new(file, "\"id\";\"name\";\"description\";\"recipe_per_stream\";\"id_constituent\";\"country\";\"type\"");

        fprintf(file, "%s", line);
        
        
        fclose(file);
        return NULL; 
    }


    Artist* artist = malloc(sizeof(Artist));
    if (!artist) return NULL;


    artist->name = g_strdup(tokens[1]);
    char* recipe_per_stream1 = remover_aspas_dup(tokens[3]);
    artist->recipe_per_stream = atof(recipe_per_stream1);
    free(recipe_per_stream1);
    artist->id_constituent = funcao(tokens[4]);
    artist->country = g_strdup(tokens[5]);
    artist->type = remover_aspas_dup(tokens[6]);
    artist->duracaoTotal = 0;
    artist->num_albuns_individual = 0;
    artist->profitTotal = 0;
    return artist; 
}