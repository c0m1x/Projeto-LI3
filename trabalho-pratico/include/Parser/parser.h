/**
 * @file parser.h
 * @brief Declarações de funções para o processamento e parsing de linhas de texto e ficheiros.
 *
 * Este arquivo define as funções necessárias para fazer o parsing de linhas de texto e carregar dados
 * para estruturas utilizando funções de construção específicas.
 */

#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Faz o parsing de uma linha de texto, dividindo-a em tokens.
 * @param line Linha de texto a ser dividida em tokens.
 * @return Um array de strings (tokens) resultante do parsing da linha.
 */
char ** parseLine(char * line);

/**
 * @brief Faz o parsing de um ficheiro, lendo linha a linha e chamando uma função de construção para cada linha.
 * @param path Caminho para o ficheiro a ser processado.
 * @param buildStruct Função de construção que será chamada para cada linha do ficheiro, passando os tokens e as estruturas necessárias.
 * @param structure1 Ponteiro para a primeira estrutura que será utilizada pela função de construção.
 * @param structure2 Ponteiro para a segunda estrutura que será utilizada pela função de construção.
 * @param structure3 Ponteiro para a terceira estrutura que será utilizada pela função de construção.
 */
void parser(char *path, void (*buildStruct)(char *, char **, void *, void *, void *, void *, void *), void *structure1, void *structure2, void *structure3, void *structure4, void *structure5);
#endif // PARSER_H
