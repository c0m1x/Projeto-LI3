#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

// Função que divide uma linha em tokens separados por um delimitador.
char **parseLine(char *line)
{
    char **tokens = malloc(sizeof(char *) * 16);
    int i = 0;
    char *token = NULL;
    while ((token = strsep(&line, ";")))
    {
        tokens[i++] = token;
    }
    tokens[i] = NULL;
    return tokens;
}

// Função que faz o parsing de um ficheiro, lendo linha a linha e chamando uma função de construção para cada linha.
void parser(char *path, void (*buildStruct)(char *, char **, void *, void *, void *, void *, void *), void *structure1, void *structure2, void *structure3, void *structure4, void *structure5)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        perror("Erro ao abrir o ficheiro");
        return;
    }
    char *line = NULL;
    ssize_t read;
    size_t len;


    read = getline(&line, &len, file);
    if (read == -1)
    {
        perror("Erro ao ler a primeira linha");
        free(line);
        fclose(file);
        return;
    }


    while ((read = getline(&line, &len, file)) != -1)
    {
        char *line_copy = strdup(line);
        char *formated_line = strsep(&line_copy, "\n");
        char **tokens = parseLine(formated_line);
        buildStruct(line, tokens, structure1, structure2, structure3, structure4, structure5);
        free(formated_line);
        free(tokens);
    }
    free(line);
    fclose(file);
}