/**
 * @file command_parser.h
 * @brief Declarações de funções para o processamento de comandos e leitura de ficheiros de entrada.
 *
 * Este arquivo define as funções necessárias para processar comandos e ler
 * os dados dos catálogos de artistas, músicas e utilizadores, e escrever os resultados em ficheiros de saída.
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Catalogos/catalogoArtist.h"
#include "../Catalogos/catalogoMusic.h"
#include "../Catalogos/catalogoUser.h"
#include "../Catalogos/catalogoAlbum.h"
#include "../Catalogos/catalogoHistory.h"

/**
 * @brief Processa um comando e executa a ação correspondente.
 * @param command Identificador do comando a ser processado.
 * @param args Argumentos do comando a ser processado.
 * @param artist_catalog Catálogo de artistas onde as informações serão consultadas.
 * @param musics_catalog Catálogo de músicas onde as informações serão consultadas.
 * @param users_catalog Catálogo de utilizadores onde as informações serão consultadas.
 * @param output_file Ficheiro onde os resultados do comando serão escritos.
 */
void process_command(char* command_str, int command_int, char** args, Artists_catalog* artist_catalog, Album_catalog* album_catalog, History_catalog* history_catalog, Musics_catalog* musics_catalog, Users_catalog* users_catalog, FILE* output_file, int **matriz);
/**
 * @brief Lê o ficheiro de entrada e carrega os dados para os catálogos de artistas, músicas e utilizadores.
 * @param filepath Caminho para o ficheiro de entrada.
 * @param artists_catalog Catálogo de artistas a ser carregado com os dados do ficheiro.
 * @param musics_catalog Catálogo de músicas a ser carregado com os dados do ficheiro.
 * @param users_catalog Catálogo de utilizadores a ser carregado com os dados do ficheiro.
 */
void read_input_file(const char* filepath, Artists_catalog* artists_catalog, Album_catalog* album_catalog, History_catalog* history_catalog, Musics_catalog* musics_catalog, Users_catalog* users_catalog, int **matriz) ;

#endif // COMMAND_PARSER_H
