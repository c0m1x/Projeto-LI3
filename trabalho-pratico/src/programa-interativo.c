#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "output_interativo.h"
/*
#include "artists.h"
#include "musics.h"
#include "users.h"
#include "command_parser.h"
#include "utils.h"
#include "catalogoUser.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "catalogoHistory.h"
#include "querie1.h"
#include "querie2.h"
#include "querie3.h"
#include "querie5.h"
*/




// Remove o caractere de nova linha (\n) do final de uma string.
void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}


void exibirMenu() {
    printf("+---------------------------------+\n");
    printf("         Grupo 71 - Projeto       \n");
    printf("+---------------------------------+\n");
    printf("Introduza o caminho da pasta com os ficheiros de dados:\n");
}



//Função principal que executa o fluxo do programa, carregando os dados, processando consultas interativas, e libertando a memória alocada.
int main() {
    char folder_path[256];
    char* result = NULL;

    exibirMenu();

    if (!fgets(folder_path, sizeof(folder_path), stdin)) {
        printf("Erro ao ler o caminho da pasta.\n");
        return 1;
    }
    remove_newline(folder_path);

    if (strlen(folder_path) == 0) {
        printf("O caminho da pasta não pode estar vazio.\n");
        return 1;
    }

    printf("Carregando os datasets e processando...\n");

    char *artists_file = build_file_path(folder_path, "artists.csv");
    char *musics_file = build_file_path(folder_path, "musics.csv");
    char *users_file = build_file_path(folder_path, "users.csv");
    char *albums_file = build_file_path(folder_path, "albums.csv");
    char *history_file = build_file_path(folder_path, "history.csv");

    if (!musics_file || !users_file || !albums_file || !history_file) {
        fprintf(stderr, "Erro ao construir caminhos dos arquivos\n");
        free(artists_file);
        free(musics_file);
        free(users_file);
        free(albums_file);
        free(history_file);
        return 1;
    }

    Artists_catalog* artists_catalog = artistsToCatalog(artists_file);
    Album_catalog* album_catalog = albumToCatalog(albums_file, artists_catalog);
    Musics_catalog* musics_catalog = musicsToCatalog(musics_file, artists_catalog, album_catalog);
    Users_catalog* users_catalog = usersToCatalog(users_file, musics_catalog);
    History_catalog* history_catalog = historyToCatalog(history_file, users_catalog, musics_catalog, artists_catalog);
    int **matriz = criar_matriz_estatica(users_catalog);

    if (!musics_catalog || !users_catalog || !history_catalog) {
        fprintf(stderr, "Erro ao processar catálogos\n");
        goto cleanup_catalogs;
    }

    printf("Dados processados com sucesso!\n");

    char query[128];
    char args[128];
    char args2[128];
    char args3[128];
    
    while (1) {
        printf("Que query deseja executar? (ou 'sair' para terminar):\n");
        if (!fgets(query, sizeof(query), stdin)) {
            printf("Erro ao ler a query.\n");
            continue;
        }
        remove_newline(query);

        if (strcmp(query, "sair") == 0) {
            printf("Saindo do programa. Obrigado!\n");
            break;
        }

        // Query 1
        if (strcmp(query, "1S") == 0 || strcmp(query, "1") == 0) {
            printf("Indique o utilizador ou artista: ");
            if (!fgets(args, sizeof(args), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args);
            
            char separador;
            if (strcmp(query, "1S") == 0) {
                separador = '=';
            } else {
                separador = ';';
            }
            
            result = output(1, separador, args, '\0', '\0',artists_catalog, NULL, users_catalog,NULL, NULL);

            if (result) {
                if (strlen(result) < 2) {
                    printf("Utilizador ou Artista não encontrado.\n");
                } else {
                    printf("\nAqui está o seu resultado da sua pesquisa:\n%s\n", result);
                }
                free(result);
                result = NULL;
            }
        }

        // Query 2
        if (strcmp(query, "2S") == 0 || strcmp(query, "2") == 0) {
            printf("Indique o número de artistas: ");
            if (!fgets(args, sizeof(args), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            if (atoi(args) <= 0) {
                printf("Coloque um número acima de 0.\n");
                continue;
            }

            remove_newline(args);
            printf("Indique o país (pressione Enter caso não escolha nenhum):\n");
            if (!fgets(args2, sizeof(args2), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args2);

            char separador;
            if (strcmp(query, "2S") == 0) {
                separador = '=';
            } else {
                separador = ';';
            }
            
            result = output(2, separador, args, args2,'\0', artists_catalog, musics_catalog, users_catalog,NULL,NULL);

            if (result) {
                if (strlen(result) < 2) {
                    printf("Não existe nenhum artista desse país.\n");
                } else {
                    printf("\nAqui está o seu resultado da sua pesquisa:\n%s\n", result);
                }
                free(result);
                result = NULL;
            }
        }

        // Query 3
        if (strcmp(query, "3S") == 0 || strcmp(query, "3") == 0) {
            printf("Indique a idade mínima: ");
            if (!fgets(args, sizeof(args), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args);
            
            printf("Indique a idade máxima: ");
            if (!fgets(args2, sizeof(args2), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args2);

            char separador;
            if (strcmp(query, "3S") == 0) {
                separador = '=';
            } else {
                separador = ';';
            }

            result = output(3, separador, args, args2, '\0',NULL, musics_catalog, users_catalog, NULL,NULL);

            if (result) {
                if (strlen(result) < 2) {
                    printf("Nenhum utilizador encontrado no intervalo especificado.\n");
                } else {
                    printf("\nAqui está o seu resultado da sua pesquisa:\n%s\n", result);
                }
                free(result);
                result = NULL;
            }
        }


        // Query 4
        if(strcmp(query,"4S")==0|| strcmp(query, "4")==0){
            result = strdup("Query não implementada\n");
        }

        // Query 5
        if (strcmp(query, "5S") == 0 || strcmp(query, "5") == 0) {
            printf("Indique o utilizador: ");
            if (!fgets(args, sizeof(args), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args);
            
            printf("Indique o número de recomendados: ");
            if (!fgets(args2, sizeof(args2), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args2);

            result = output(5, '\0', args, args2,'\0', NULL, NULL, users_catalog,NULL, matriz);

            if (result) {
                if (strlen(result) < 2) {
                    printf("Nenhum resultado encontrado para o utilizador especificado.\n");
                } else {
                    printf("\nAqui está o seu resultado da sua pesquisa:\n%s\n", result);
                }
                free(result);
                result = NULL;
            }
        }



        // Query 6
        if (strcmp(query, "6S") == 0 || strcmp(query, "6") == 0) {
            printf("Indique o utilizador: ");
            if (!fgets(args, sizeof(args), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args);
            
            printf("Indique o ano: ");
            if (!fgets(args2, sizeof(args2), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args2);
            
            printf("Indique o número de artistas(opcional): ");
            if (!fgets(args3, sizeof(args3), stdin)) {
                printf("Erro ao ler os argumentos.\n");
                continue;
            }
            remove_newline(args3);

            char separador;
            if (strcmp(query, "6S") == 0) {
                separador = '=';
            } else {
                separador = ';';
            }
            if(strlen(args3) < 1){
                args3[0] = '\0';
                result = output(6, separador , args, args2,args3, NULL, musics_catalog, NULL,history_catalog, NULL);
            }
            else{
                result = output(6, separador , args, args2,args3, NULL, musics_catalog, NULL,history_catalog, NULL);
            }


            if (result) {
                if (strlen(result) < 2) {
                    printf("Nenhum resultado encontrado para o utilizador especificado.\n");
                } else {
                    printf("\nAqui está o seu resultado da sua pesquisa:\n%s\n", result);
                }
                free(result);
                result = NULL;
            }
        }
    }

cleanup_catalogs:
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

    return 0;
}

