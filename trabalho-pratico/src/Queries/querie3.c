#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "users.h"
#include "musics.h"
#include "catalogoUser.h"
#include "catalogoMusic.h"
#include "utils.h"

// Estrutura que representa um género e a sua popularidade.
typedef struct genrePopularity {
    char* genre;
    int total_likes;
} GenrePopularity;

// Formata as informações de um género para uma string de saída.
char* formatGenreOutput(const GenrePopularity* genre_popularity, char separador) {
    if (!genre_popularity) {
        return strdup("");
    }


    char* genre_no_quotes = remover_aspas_dup(genre_popularity->genre);

    const char* format = (separador == ';') ? "%s;%d\n" : "%s=%d\n";

    int len = snprintf(NULL, 0, format, genre_no_quotes, genre_popularity->total_likes);
    char* output = malloc(len + 1);

    if (output) {
        snprintf(output, len + 1, format, genre_no_quotes, genre_popularity->total_likes);
    }

    free(genre_no_quotes); 
    return output;
}


// Função de comparação usada para ordenar os géneros por popularidade.
int compare_genres(const void* a, const void* b) {
    GenrePopularity* genreA = (GenrePopularity*) a;
    GenrePopularity* genreB = (GenrePopularity*) b;

    if (genreA->total_likes != genreB->total_likes) {
        return genreB->total_likes - genreA->total_likes;
    }
    return strcmp(genreA->genre, genreB->genre);
}

// Liberta a memória da estrutura GenrePopularity.
void free_genre_popularity(gpointer data) {
    GenrePopularity* genre_popularity = (GenrePopularity*) data;
    free(genre_popularity->genre);  
    free(genre_popularity);
}

// Função auxiliar para remover caracteres indesejados de uma string.
void remove_chars(char* str) {
    int i, j = 0;
    int len = strlen(str);

    for (i = 0; i < len; i++) {
        if (str[i] != '\'' && str[i] != '[' && str[i] != ']' && str[i] != '\"' && str[i] != ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Função principal da Query 3, que determina a popularidade dos géneros musicais com base na faixa etária dos utilizadores.
char* querie3(int min_age, int max_age, Users_catalog* users_catalog, Musics_catalog* musics_catalog, char separador) {
    GHashTable* genres_popularity = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free_genre_popularity);
    GHashTable* users_table = get_users_hash_table(users_catalog);
    GHashTable* musics_table = get_musics_hash_table(musics_catalog);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, users_table);

    int found_user_in_age_range = 0;

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        User* user = (User*) value;
        if (!user) continue;

        int age = get_age(user);

        if (age >= min_age && age <= max_age) {
            found_user_in_age_range = 1;

            const GArray* liked_songs = get_liked_songs_id(user);
            if (!liked_songs) continue;

            for (guint i = 0; i < liked_songs->len; i++) {
                char* quoted_song_id = g_array_index(liked_songs, char*, i);

                if (!quoted_song_id) {
                    fprintf(stderr, "Erro de alocação para quoted_song_id\n");
                    exit(EXIT_FAILURE);
                }

                Music* liked_music = (Music*) g_hash_table_lookup(musics_table, quoted_song_id);
                //free(quoted_song_id);
                if (!liked_music) continue;

                char* genre = get_music_genre(liked_music);
                if (!genre) continue;

                GenrePopularity* genre_popularity = (GenrePopularity*) g_hash_table_lookup(genres_popularity, genre);
                if (!genre_popularity) {
                    genre_popularity = malloc(sizeof(GenrePopularity));
                    if (!genre_popularity) {
                        fprintf(stderr, "Erro de alocação para GenrePopularity\n");
                        exit(EXIT_FAILURE);
                    }
                    genre_popularity->genre = strdup(genre);
                    genre_popularity->total_likes = 0;
                    g_hash_table_insert(genres_popularity, genre_popularity->genre, genre_popularity);
                }
                genre_popularity->total_likes++;
                free(genre);
            }
        }
    }

    if (!found_user_in_age_range || g_hash_table_size(genres_popularity) == 0) {
        g_hash_table_destroy(genres_popularity);
        return NULL;
    }

    int num_genres = g_hash_table_size(genres_popularity);
    GenrePopularity* genre_array = malloc(num_genres * sizeof(GenrePopularity));
        
    if (!genre_array) {
        g_hash_table_destroy(genres_popularity);
        return NULL;
    }

    GHashTableIter iter_genres;
    gpointer key_genre, value_genre;
    int index = 0;

    g_hash_table_iter_init(&iter_genres, genres_popularity);
    while (g_hash_table_iter_next(&iter_genres, &key_genre, &value_genre)) {
        GenrePopularity* genre_popularity = (GenrePopularity*) value_genre;
        genre_array[index++] = *genre_popularity;
    }

    qsort(genre_array, num_genres, sizeof(GenrePopularity), compare_genres);

    size_t output_len = 1; 
    char** formatted_genres = malloc(num_genres * sizeof(char*));
    if (!formatted_genres) {
        free(genre_array);
        g_hash_table_destroy(genres_popularity);
        return NULL;
    }

    for (int i = 0; i < num_genres; i++) {
        formatted_genres[i] = formatGenreOutput(&genre_array[i], separador);
        if (formatted_genres[i]) {
            output_len += strlen(formatted_genres[i]);
        }
    }

    char* result = malloc(output_len);
    if (!result) {
        for (int i = 0; i < num_genres; i++) {
            free(formatted_genres[i]);
        }
        free(formatted_genres);
        free(genre_array);
        g_hash_table_destroy(genres_popularity);
        return NULL;
    }

    result[0] = '\0';
    for (int i = 0; i < num_genres; i++) {
        if (formatted_genres[i]) {
            strcat(result, formatted_genres[i]);
            free(formatted_genres[i]);
        }
    }

    free(formatted_genres);
    free(genre_array);
    g_hash_table_destroy(genres_popularity);


    return result;
}

