#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "history.h"
#include "parser.h"
#include "catalogoHistory.h"

#include "utils.h"

// Estrutura que representa um catálogo de históricos organizados por ano.
typedef struct history_catalog {
    GHashTable* years; 
} History_catalog;


// Obtém a tabela hash de anos do catálogo de históricos.
GHashTable* get_years_table(History_catalog* catalogo) {
    return catalogo->years;
}

void buildHistory(char* line, char** tokens, History_catalog* catalog, Users_catalog* user_catalog, Musics_catalog* music_catalog, Artists_catalog* artist_catalog) {
    History* history = completa_History(line, tokens);
    if (!history) return;

    char* user_id = strdup(tokens[1]);
    char* year = strndup(tokens[3] + 1, 4);

    char* music_id = get_history_music_id(history);
    
    char* genero = getMusic_genre(music_catalog, music_id);

    if (!genero) return; 

    
    int genero_int = genero_to_int(genero);

    add_genero_User_catalog(user_catalog,user_id, genero_int);

    const GArray* artist_ids = get_artist_music_catalog(music_catalog, music_id);

    for(int i = 0; i < artist_ids->len; i++) {

        char* current_artist = g_array_index(artist_ids, char*, i);
        
        add_profit_artist_catalog(artist_catalog, current_artist);
    }


    free(music_id);
    free(genero);
    free(user_id);

    char* user_id_sem_aspas = remover_aspas_dup(tokens[1]);
    YearHistories* year_histories = g_hash_table_lookup(catalog->years, year);
    if (!year_histories) {
    year_histories = malloc_year(user_id_sem_aspas, history);
    if (!year_histories) {
        free(user_id_sem_aspas);
        free(year);
        return;
    }
    g_hash_table_insert(catalog->years, g_strdup(year), year_histories);
} else {

    set_user_in_year(year_histories, user_id_sem_aspas, history);
}

    free(year);
    free(user_id_sem_aspas);
}

// Wrapper para uso com a função parser, chamando buildHistory.
void buildHistory_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) {
    return buildHistory(line, tokens, (History_catalog *)structure1, (Users_catalog *)structure2, (Musics_catalog *)structure3, (Artists_catalog *) structure4);
}

// Cria e carrega um catálogo de históricos a partir de um arquivo CSV.
History_catalog* historyToCatalog(char* path, void* structure1, void* structure2, void* structure4) {
    History_catalog* catalog = malloc(sizeof(History_catalog));
    if (!catalog) return NULL;

    catalog->years = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_year_histories_void);

    parser(path, buildHistory_wrapper, catalog, structure1, structure2, structure4, NULL);
    return catalog;
}
// Liberta a memória associada ao catálogo de históricos.
void free_history_catalog(History_catalog* catalog) {
    if (catalog) {
        g_hash_table_destroy(catalog->years);

        free(catalog);
    }
}

