#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <ctype.h>
#include "command_parser.h"
#include "utils.h"
#include "catalogoUser.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "catalogoHistory.h"
#include "output_interativo.h"



struct QueryStats {
    int total_tests;
    int correct_tests;
    double total_time;
    struct Error {
        int query_number;
        int line_number;
        char filename[256];
    } *errors;
    int error_count;
};

//Função que calcula o tempo decorrido entre dois instantes
double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

//Função que imprime as estatisticas de uma query
void print_query_stats(struct QueryStats stats[], int query_num) {
    printf("Q%d: %d de %d testes ok!\n", query_num + 1, 
           stats[query_num].correct_tests, 
           stats[query_num].total_tests);
    
    if (stats[query_num].error_count > 0) {
        for (int i = 0; i < stats[query_num].error_count; i++) {
            printf("Descrepância em command%d_output: linha %d \n",
                   stats[query_num].errors[i].query_number,
                   stats[query_num].errors[i].line_number);
        }
    }
    printf("\n");
}

//Remove os espaços em branco no início e no final de uma string.
void trim_whitespace(char* str) {
    char* end;

    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }


    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
}

//Função que compara o conteúdo de dois ficheiros
int compare_files(char* expected_file, char* result, int query_number, struct QueryStats* stats) {
    FILE *expected = fopen(expected_file, "r");
    if (!expected) return -1;

    fseek(expected, 0, SEEK_END);
    long file_size = ftell(expected);
    rewind(expected);


    char* file_content = malloc(file_size + 1);
    if (!file_content) {
        fclose(expected);
        return -1;
    }
    
    size_t read_bytes = fread(file_content, 1, file_size, expected);
    if (read_bytes != (size_t)file_size) {
        perror("Error reading file");
        free(file_content);
        fclose(expected);
        return -1;
    }

    file_content[file_size] = '\0';

    
    if (strcmp(result, "\n") == 0 && strcmp(file_content, "\n") == 0) {
        free(file_content);
        fclose(expected);
        return 0;
    }

    
    int is_different = strcmp(file_content, result);
    
    
    if (is_different && stats->error_count < 100) {
        stats->errors[stats->error_count].query_number = query_number;
        stats->errors[stats->error_count].line_number = 1;
        strncpy(stats->errors[stats->error_count].filename, expected_file, 255);
        stats->error_count++;
    }

    free(file_content);
    fclose(expected);
    return is_different;
}

//Retorna a quantidade de memória usada pelo processo atual em megabytes.
double get_memory_mb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024.0; 
}


//Lê o conteúdo de um arquivo e retorna esse conteúdo como uma string.
char* read_file_to_string(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* content = malloc(length + 1);
    if (!content) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    size_t read_bytes = fread(content, 1, length, file);
    if (read_bytes != (size_t)length) {
        perror("Error reading file");
        free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';

    fclose(file);
    return content;
}




int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s <csv_folder> <input_file> <expected_output_folder>\n", argv[0]);
        return 1;
    }
    char* result = NULL;

    const char* folder_path = argv[1];
    const char* input_file = argv[2];
    const char* expected_output_folder = argv[3];

    char* artists_file = build_file_path(folder_path, "artists.csv");
    char* musics_file = build_file_path(folder_path, "musics.csv");
    char* users_file = build_file_path(folder_path, "users.csv");
    char* albums_file = build_file_path(folder_path, "albums.csv");
    char* history_file = build_file_path(folder_path, "history.csv");

    Artists_catalog* artists_catalog = artistsToCatalog(artists_file);
    Album_catalog* album_catalog = albumToCatalog(albums_file, artists_catalog);
    Musics_catalog* musics_catalog = musicsToCatalog(musics_file, artists_catalog, album_catalog);
    Users_catalog* users_catalog = usersToCatalog(users_file, musics_catalog);
    History_catalog* history_catalog = historyToCatalog(history_file, users_catalog, musics_catalog, artists_catalog);
    int** matriz = criar_matriz_estatica(users_catalog);

    FILE* input = fopen(input_file, "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    struct QueryStats stats[6] = {0};
    for (int i = 0; i < 6; i++) {
        stats[i].errors = malloc(100 * sizeof(struct Error));
        stats[i].error_count = 0;
        stats[i].total_tests = 0;
        stats[i].correct_tests = 0;
        stats[i].total_time = 0.0;
    }

    struct timespec global_start, global_end;
    clock_gettime(CLOCK_REALTIME, &global_start);

    char command_line[256];
    int command_num = 1;
    char separador;

    while (fgets(command_line, sizeof(command_line), input)) {
        command_line[strcspn(command_line, "\n")] = '\0';
        char command_copy[256];
        strcpy(command_copy, command_line);

        int query_type = -1;
        if (command_line[0] >= '1' && command_line[0] <= '6') {
            query_type = command_line[0] - '1';
            separador = command_line[1] == 'S' ? '=' : ';';
        }

        if (query_type < 0 || query_type >= 6) continue;

        char *args = NULL, *args2 = NULL, *args3 = NULL;
        char *command_str = strtok(command_copy, " ");
        
        switch (query_type + 1) {
            case 2:
                args = command_str;
                args2 = strtok(NULL, " ");
                args3 = args2 ? strtok(NULL, "\n") : NULL;
                break;
            case 3:
                args = strtok(NULL, " ");
                args2 = strtok(NULL, " ");
                break;
            default:
                args = strtok(NULL, " ");
                args2 = strtok(NULL, " ");
                args3 = strtok(NULL, " ");
        }

        char expected_output_path[256];
        snprintf(expected_output_path, sizeof(expected_output_path), 
                "%s/command%d_output.txt", expected_output_folder, command_num);

        struct timespec query_start, query_end;
        clock_gettime(CLOCK_REALTIME, &query_start);

        
        if(query_type +1 ==2){
                if(!args3){
                result = output(2, separador, args2, args3, '\0', 
                            artists_catalog, musics_catalog, users_catalog, 
                            history_catalog, matriz);
                }
                else{

            char* args4 = remover_aspas_dup(args3);
            result = output(2, separador, args2, args4, '\0', 
                             artists_catalog, musics_catalog, users_catalog, 
                             history_catalog, matriz);
            free(args4);
                }
        }
        else{
            result = output(query_type + 1, separador, args, args2, args3, 
                            artists_catalog, musics_catalog, users_catalog, 
                            history_catalog, matriz);
        }

        if(!result){
            result = strdup("\n");
        }


        clock_gettime(CLOCK_REALTIME, &query_end);
        stats[query_type].total_tests++;
        stats[query_type].total_time += get_elapsed_time(query_start, query_end);

        if (result) {
            if (compare_files(expected_output_path, result, command_num, &stats[query_type]) == 0) {
                stats[query_type].correct_tests++;
            }
            free(result);
        }

        command_num++;
    }

    clock_gettime(CLOCK_REALTIME, &global_end);
    double total_elapsed = get_elapsed_time(global_start, global_end);


    for (int i = 0; i < 6; i++) {
        if (stats[i].total_tests > 0) {
            print_query_stats(stats, i);
        }
    }

    printf("Memória utilizada: %.0f MB\n", get_memory_mb());
    printf("Tempos de execução:\n");
    for (int i = 0; i < 6; i++) {
        if (stats[i].total_tests > 0) {
            printf("Q%d: %.6f ms\n", i + 1, 
                   stats[i].total_time * 1000.0 / stats[i].total_tests);
        }
    }
    printf("Tempo total: %.0fs\n", total_elapsed);

    for (int i = 0; i < 6; i++) {
        free(stats[i].errors);
    }

    if(matriz) free_matriz(matriz, users_catalog);
    if (artists_catalog) free_artists_catalog(artists_catalog);
    if (musics_catalog) free_musics_catalog(musics_catalog);
    if (users_catalog) free_users_catalog(users_catalog);
    if (album_catalog) free_album_catalog(album_catalog);
    if (history_catalog) free_history_catalog(history_catalog);

    free(artists_file);
    free(musics_file);
    free(users_file);
    free(albums_file);
    free(history_file);
    fclose(input);

    return 0;
}