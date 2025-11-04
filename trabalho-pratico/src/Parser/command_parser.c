#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "command_parser.h"
#include "artists.h"
#include "musics.h"      
#include "users.h"
#include "album.h"
#include "history.h"
#include "querie1.h"
#include "querie2.h"
#include "querie3.h"
#include "querie5.h"
#include "querie6.h"

// Processa um comando e executa a ação correspondente.
void process_command(char* command_str, int command_int, char** args, Artists_catalog* artist_catalog, Album_catalog* album_catalog, History_catalog* history_catalog, Musics_catalog* musics_catalog, Users_catalog* users_catalog, FILE* output_file, int **matriz) {
    char* result = NULL;

    
    if (strcmp(command_str, "1S") == 0) {
        result = querie1(args[0], users_catalog, artist_catalog, '=');
        goto print_result;
    }

    if (strcmp(command_str, "2S") == 0) {
        int N = atoi(args[1]);
        const char* country_filter = args[2];
        result = (N == 0) ? strdup("\n") : querie2(artist_catalog, musics_catalog, N, country_filter, '=');
        goto print_result;
    }

    if (strcmp(command_str, "3S") == 0) {
        int min_age = atoi(args[0]);
        int max_age = atoi(args[1]);
        result = querie3(min_age, max_age, users_catalog, musics_catalog, '=');
        goto print_result;
    }

    if(strcmp(command_str, "4S") == 0){
        result =strdup ("\n");
        goto print_result;
    }

    if (strcmp(command_str, "5S") == 0) {
        if (args[0] && args[1]) {
            char* id_alvo = args[0];
            int args_int = atoi(args[1]);
            result = querie5(users_catalog, id_alvo, args_int, matriz);
        } else {
            result = strdup("Comando 5 requer dois argumentos (user e numero de users recomendados).\n");
        }
        goto print_result;
    }

    if(strcmp(command_str, "6S") == 0){
        
        if(!args[2]){
            char* id_alvo = args[0];
            char* year = args[1];
            result = querie6('=',id_alvo, year, -1 ,musics_catalog, history_catalog);
        }
        else if(args[0] && args[1] && args[2]){

            char* id_alvo = args[0];
            char* year = args[1];
            int N = atoi(args[2]);
            result = querie6('=',id_alvo, year, N,musics_catalog, history_catalog);
        }
        else{
            result = strdup("Comando 6 requer dois argumentos (user e ano) ou três argumentos (user, ano e N).\n");
        }
        
            goto print_result;
    }

    
    switch (command_int) {
        case 1:
            result = (args[0]) ? querie1(args[0], users_catalog, artist_catalog, ';') : strdup("Comando 1 requer um argumento (user_id).\n");
            goto print_result;

        case 2: {
            if (args[0] && args[1]) {
                int N = atoi(args[1]);
                const char* country_filter = args[2];
                result = (N == 0) ? strdup("\n") : querie2(artist_catalog, musics_catalog, N, country_filter, ';');
            } else {
                result = strdup("Comando 2 requer dois argumentos (N, country_filter).\n");
            }
            goto print_result;
        }

        case 3:
            if (args[0] && args[1]) {
                int min_age = atoi(args[0]);
                int max_age = atoi(args[1]);
                result = querie3(min_age, max_age, users_catalog, musics_catalog, ';');
            } else {
                result = strdup("Comando 3 requer dois argumentos (min_age e max_age).\n");
            }
            goto print_result;

        case 4: 
            result = strdup("\n");
            goto print_result;

        case 5:
            if (args[0] && args[1]) {
                char* id_alvo = args[0];
                int args_int = atoi(args[1]);
                result = querie5(users_catalog, id_alvo, args_int, matriz);
            } else {
                result = strdup("Comando 5 requer dois argumentos (user e numero de users recomendados).\n");
            }
            goto print_result;
        
        case 6:
    
        if(!args[2]){
            char* id_alvo = args[0];
            char* year = args[1];
            result = querie6(';',id_alvo, year, -1 ,musics_catalog, history_catalog);
        }
        else if(args[0] && args[1] && args[2]){
            char* id_alvo = args[0];
            char* year = args[1];
            int N = atoi(args[2]);
            result = querie6(';', id_alvo, year, N,musics_catalog, history_catalog);
        }
        else{
            result = strdup("Comando 6 requer dois argumentos (user e ano) ou três argumentos (user, ano e N).\n");
        }
            goto print_result;

        default:
            result = strdup("Comando inválido.\n");
            goto print_result;
    }

print_result:
    if (output_file) {
        fprintf(output_file, "%s", result ? result : "\n");
    } else {
        fprintf(stderr, "Erro: Arquivo de saída não fornecido.\n");
    }
    free(result);
}



// Lê um arquivo de comandos e executa cada comando.
void read_input_file(const char* filepath, Artists_catalog* artists_catalog, Album_catalog* album_catalog, History_catalog* history_catalog, Musics_catalog* musics_catalog, Users_catalog* users_catalog, int **matriz) {
        FILE* file = fopen(filepath, "r");
        if (!file) {
            perror("Erro ao abrir o ficheiro de comandos");
            return;
        }

        char line[256];
        int command_number = 1;

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;

            char* command_str = strtok(line, " ");
            if (!command_str) continue;

            int command = atoi(command_str);

            char* args[10] = {NULL};
            int i = 0;

            if (command == 2) {
                args[0] = command_str;
                args[1] = strtok(NULL, " ");
                if (args[1]) {
                    args[2] = strtok(NULL, "\n");
                }
            } else if (command == 3) {
                args[0] = strtok(NULL, " ");
                args[1] = strtok(NULL, " ");
            } else {
                char* arg = strtok(NULL, " ");
                while (arg && i < 10) {
                    args[i++] = arg;
                    arg = strtok(NULL, " ");
                }
            }

            char output_filename[256];
            sprintf(output_filename, "resultados/command%d_output.txt", command_number);
            FILE* output_file = fopen(output_filename, "w");
            if (!output_file) {
                perror("Erro ao criar o ficheiro de saída");
                continue;
            }

            process_command(command_str, command, args, artists_catalog, album_catalog, history_catalog, musics_catalog, users_catalog, output_file, matriz);

            fclose(output_file);
            command_number++;
        }

        fclose(file);
    }

