#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artists.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "utils.h"

// Função que formata a informação de um artista para output.
char* formatArtistOutput(const Artist* artist, char separador) {
    if (!artist) return NULL;

    char* name = get_artist_name(artist);

    char* type = get_artist_type(artist);

    char* raw_country = get_artist_country(artist);
    char* country = remover_aspas_dup(raw_country);
    int total_duration = get_artist_duracaoTotal(artist);
    char* total_duration_str = utils_segundos_para_string(total_duration);

    const char* format = (separador == ';') ? "%s;%s;%s;%s\n" : "%s=%s=%s=%s\n";

    int len = snprintf(NULL, 0, format, name, type, total_duration_str, country);
    char* output = malloc(len + 1);
    if (output) {
        snprintf(output, len + 1, format, name, type, total_duration_str, country);
    }


    free(total_duration_str);
    free(name);
    free(type);
    free(raw_country);
    free(country);

    return output;
}

// Função de comparação para ordenar artistas por duração total de músicas e, em caso de empate, por nome.
int compare_artists_by_total_duration_then_name(gconstpointer a, gconstpointer b) {
    const Artist *artistaA = *(const Artist **)a;
    const Artist *artistaB = *(const Artist **)b;

    int duracaoA = get_artist_duracaoTotal(artistaA);
    int duracaoB = get_artist_duracaoTotal(artistaB);

    if (duracaoA != duracaoB) {
        return duracaoB - duracaoA;
    }

    char* artista_nameA = get_artist_name(artistaA);
    char* artista_nameB = get_artist_name(artistaB);
    int result = strcmp(artista_nameA, artista_nameB);

    free(artista_nameA);
    free(artista_nameB);

    return result;
}

// Executa a Query 2, listando os artistas mais relevantes com base na duração total de suas músicas.
char* querie2(Artists_catalog* catalogo, Musics_catalog* catalogo2, int N, const char* country_filter, char separador) {
    GHashTable* artists_table = get_artists_table(catalogo);
    if (N <= 0) return NULL;

    GArray *filtered_artists = g_array_new(FALSE, FALSE, sizeof(Artist*));
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, artists_table);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Artist *artista = (Artist *)value;
        char *artist_country = get_artist_country(artista);
        if (!country_filter || (artist_country && strcmp(artist_country, country_filter) == 0)) {
            g_array_append_val(filtered_artists, artista);
        }
        g_free(artist_country);
    }

    if (filtered_artists->len == 0) {
        g_array_free(filtered_artists, TRUE);
        return NULL;
    }

    g_array_sort(filtered_artists, compare_artists_by_total_duration_then_name);

    char* final_output = NULL;
    size_t final_length = 0;

    for (int i = 0; i < MIN(N, filtered_artists->len); i++) {
        Artist *artista = g_array_index(filtered_artists, Artist*, i);
        char* formatted_output = formatArtistOutput(artista, separador);
        if (!formatted_output) continue;

        size_t add_len = strlen(formatted_output);

        char* temp_output = realloc(final_output, final_length + add_len + 1);
        if (!temp_output) {
            free(formatted_output);
            free(final_output);
            g_array_free(filtered_artists, TRUE);
            return NULL;
        }
        final_output = temp_output;

        if (final_length == 0) {
            strcpy(final_output, formatted_output);
        } else {
            strcat(final_output, formatted_output);
        }
        final_length += add_len;

        free(formatted_output);
    }

    g_array_free(filtered_artists, TRUE);

    return final_output ? final_output : NULL;
}
