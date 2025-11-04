#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "users.h"
#include "musics.h"
#include "catalogoMusic.h"
#include "catalogoUser.h"
#include "catalogoHistory.h"
#include "utils.h"


typedef struct {
    unsigned long total_time;
    int unique_music_count;
    GHashTable* artist_time;
    GHashTable* artist_music_count;
    GHashTable* genre_time;
    GHashTable* album_time;
    GHashTable* day_count;
    float hour_count[24];
} Statistics;

// Estrutura auxiliar 
typedef struct {
    char* artist;
    unsigned long time;
    int music_count;
} ArtistTime;


// Inicializa a estrutura de estatísticas.
Statistics* init_statistics() {
    Statistics* stats = malloc(sizeof(Statistics));
    stats->total_time = 0;
    stats->unique_music_count = 0;
    stats->artist_time = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
    stats->artist_music_count = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
    stats->genre_time = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
    stats->album_time = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
    stats->day_count = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
    memset(stats->hour_count, 0.0, sizeof(stats->hour_count));
    return stats;
}

// Função auxiliar para libertar a memória de uma estrutura de estatísticas.
void free_statistics(Statistics* stats) {
    g_hash_table_destroy(stats->artist_time);
    g_hash_table_destroy(stats->artist_music_count);
    g_hash_table_destroy(stats->genre_time);
    g_hash_table_destroy(stats->album_time);
    g_hash_table_destroy(stats->day_count);
    free(stats);
}

// Função auxiliar para converter uma duração de música 
int convert_duration2(const char* duration_str) {
    if (!duration_str || !*duration_str) return 0;
    
    int hours = 0, minutes = 0, seconds = 0;
    char* clean_duration = remover_aspas_dup(duration_str);
    int result = 0;
    
    if (sscanf(clean_duration, "%d:%d:%d", &hours, &minutes, &seconds) == 3) {
        result = hours * 3600 + minutes * 60 + seconds;
    } else if (sscanf(clean_duration, "%d:%d", &minutes, &seconds) == 2) {
        result = minutes * 60 + seconds;
    } else if (sscanf(clean_duration, "%d", &seconds) == 1) {
        result = seconds;
    }
    
    free(clean_duration);
    return result;
}

// Função auxiliar para atualizar o contador de tempo.
static void update_time_counter(GHashTable* table, const char* key, unsigned long duration) {
    unsigned long* time = g_hash_table_lookup(table, key);
    if (!time) {
        time = calloc(1, sizeof(unsigned long));
        g_hash_table_insert(table, g_strdup(key), time);
    }
    *time += duration;
}

//Extrai o identificador numérico de um artista a partir de uma string, ignorando aspas e prefixos.
static int extract_artist_id(const char* artist_str) {
    char* clean_str = remover_aspas_dup(artist_str);
    int id = atoi(clean_str + 1);
    free(clean_str);
    return id;
}

// Função auxiliar para comparar artistas por tempo e, em caso de empate, por ID.
static int compare_artists_by_time(const void* a, const void* b) {
    const ArtistTime* artist_a = (const ArtistTime*)a;
    const ArtistTime* artist_b = (const ArtistTime*)b;
    
    if (artist_b->time != artist_a->time) {
        return (artist_b->time > artist_a->time) ? 1 : -1;
    }
    
    int id_a = extract_artist_id(artist_a->artist);
    int id_b = extract_artist_id(artist_b->artist);
    return id_a - id_b;
}

//Função auxiliar para encontrar a entrada com o maior valor numa tabela hash.
static char* find_max_entry(GHashTable* table, unsigned long* max_value, const char* type) {
    char* max_key = NULL;
    *max_value = 0;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, table);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        unsigned long* current = value;
        if (*current > *max_value) {
            max_key = key;
            *max_value = *current;
        } else if (*current == *max_value && max_key) {
            if (strcmp(type, "artist") == 0) {
                int curr_id = extract_artist_id(key);
                int max_id = extract_artist_id(max_key);
                if (curr_id < max_id) {
                    max_key = key;
                }
            } 
            else if (strcmp(type, "day") == 0) {
                int year1, month1, day1, year2, month2, day2;
                sscanf(key, "%d/%d/%d", &year1, &month1, &day1);
                sscanf(max_key, "%d/%d/%d", &year2, &month2, &day2);
                
                if (year1 > year2 || 
                    (year1 == year2 && month1 > month2) ||
                    (year1 == year2 && month1 == month2 && day1 > day2)) {
                    max_key = key;
                }
            } else {
                if (strcmp(key, max_key) < 0) {
                    max_key = key;
                }
            }
        }
    }
    
    return max_key;
}

// Função auxiliar para formatar uma string de estatísticas.
char* format_statistics(char separador, Statistics* stats, int N) {
    if (stats->total_time == 0) return strdup("\n");
    
    char* total_time_str = utils_segundos_para_string(stats->total_time);
    
    unsigned long max_value;
    char* top_artist = find_max_entry(stats->artist_time, &max_value, "artist");
    char* top_genre = find_max_entry(stats->genre_time, &max_value, "genre");
    char* top_album = find_max_entry(stats->album_time, &max_value, "album");
    char* top_day = find_max_entry(stats->day_count, &max_value, "day");
    
    int top_hour = 0, max_hour_count = 0;
    for (int i = 0; i < 24; i++) {
        if (stats->hour_count[i] > max_hour_count || 
            (stats->hour_count[i] == max_hour_count && i < top_hour)) {
            top_hour = i;
            max_hour_count = stats->hour_count[i];
        }
    }
    
    char* top_artist_clean = remover_aspas_dup(top_artist);
    char* top_genre_clean = remover_aspas_dup(top_genre);
    char* top_album_clean = remover_aspas_dup(top_album);
    
    GString* result = g_string_new(NULL);
    g_string_append_printf(result, "%s%c%d%c%s%c%s%c%s%c%s%c%02d\n",
                         total_time_str,
                         separador,
                         stats->unique_music_count,
                         separador,
                         top_artist_clean ? top_artist_clean : "",
                         separador,
                         top_day ? top_day : "",
                         separador,
                         top_genre_clean ? top_genre_clean : "",
                         separador,
                         top_album_clean ? top_album_clean : "",
                         separador,
                         top_hour);
    
    if (N > 0) {
        ArtistTime* artist_times = malloc(g_hash_table_size(stats->artist_time) * sizeof(ArtistTime));
        int artist_count = 0;
        
        GHashTableIter iter;
        gpointer key, value;
        g_hash_table_iter_init(&iter, stats->artist_time);
        
        while (g_hash_table_iter_next(&iter, &key, &value)) {
            artist_times[artist_count].artist = g_strdup(key);
            artist_times[artist_count].time = *(unsigned long*)value;
            int* music_count = g_hash_table_lookup(stats->artist_music_count, key);
            artist_times[artist_count].music_count = music_count ? *music_count : 0;
            artist_count++;
        }
        
        qsort(artist_times, artist_count, sizeof(ArtistTime), compare_artists_by_time);
        
        for (int i = 0; i < MIN(N, artist_count); i++) {
            char* artist_clean = remover_aspas_dup(artist_times[i].artist);
            char* time_str = utils_segundos_para_string(artist_times[i].time);
            
            g_string_append_printf(result, "%s%c%d%c%s\n",
                                artist_clean,
                                separador,
                                artist_times[i].music_count,
                                separador,
                                time_str);
            
            free(artist_clean);
            free(time_str);
        }
        
        for (int i = 0; i < artist_count; i++) {
            g_free(artist_times[i].artist);
        }
        free(artist_times);
    }
    
    free(total_time_str);
    free(top_artist_clean);
    free(top_genre_clean);
    free(top_album_clean);
    
    return g_string_free(result, FALSE);
}

// Função principal para executar a Query 6.
char* querie6(char separador, char* query_user, char* query_year, int N,
              Musics_catalog* music_catalog, History_catalog* history_catalog) {
    
    YearHistories* year_histories = g_hash_table_lookup(get_years_table(history_catalog), query_year);
    if (!year_histories) return strdup("\n");
    
    UserHistories* users_table = get_users_table_by_year(year_histories, query_user);
    if (!users_table) return strdup("\n");
    
    GHashTable* user_history_table = get_user_histories_table(users_table);
    if (!user_history_table) return strdup("\n");
    
    Statistics* stats = init_statistics();
    GHashTable* unique_musics = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTable* musics_table = get_musics_hash_table(music_catalog);
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, user_history_table);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        History* history = (History*)value;
        char* music_id = get_history_music_id(history);
        Music* music = g_hash_table_lookup(musics_table, music_id);
        
        if (music) {
            char* history_duration = get_history_duration(history);
            unsigned long duration = convert_duration2(history_duration);
            stats->total_time += duration;
            free(history_duration);
            
            if (!g_hash_table_contains(unique_musics, music_id)) {
                g_hash_table_add(unique_musics, strdup(music_id));
                const GArray* artists = get_music_artist_idss(music);
                for (size_t i = 0; i < artists->len; i++) {
                    int* count = g_hash_table_lookup(stats->artist_music_count, g_array_index(artists, char*, i));
                    if (!count) {
                        count = calloc(1, sizeof(int));
                        g_hash_table_insert(stats->artist_music_count, 
                            g_strdup(g_array_index(artists, char*, i)), count);
                    }
                    (*count)++;
                }
            }
            
            const GArray* artists = get_music_artist_idss(music);
            for (size_t i = 0; i < artists->len; i++) {
                update_time_counter(stats->artist_time, g_array_index(artists, char*, i), duration);
            }
            
            char* music_genre = get_music_genre(music);
            char* music_album = get_music_album(music);
            update_time_counter(stats->genre_time, music_genre, duration);
            update_time_counter(stats->album_time, music_album, duration);

            free(music_genre);
            free(music_album);
            
            char* history_month = get_history_month(history);
            char* history_day = get_history_day(history);
            char* day_key = g_strdup_printf("%s/%s/%s",query_year, history_month, history_day);
            free(history_month);
            free(history_day);
            
            int* count = g_hash_table_lookup(stats->day_count, day_key);
            if (!count) {
                count = calloc(1, sizeof(int));
                g_hash_table_insert(stats->day_count, day_key, count);
            } else {
                g_free(day_key);
            }
            (*count)++;
            char* history_hour = get_history_hour(history);
            stats->hour_count[atoi(history_hour)] += duration;
            free(history_hour);
        }
        stats->unique_music_count = g_hash_table_size(unique_musics);
        free(music_id);
    }
    
    char* result = format_statistics(separador, stats, N);
    
    g_hash_table_destroy(unique_musics);
    free_statistics(stats);
    
    return result;
}