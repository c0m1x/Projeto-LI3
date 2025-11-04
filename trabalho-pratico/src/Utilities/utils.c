#include <stdio.h>
#include <glib.h>
#include <stdlib.h>

// Função auxiliar para escrever a primeira linha com o cabeçalho, se necessário
void write_csv_header_if_new(FILE *file, const char *header) {
    long current_pos = ftell(file); // Guarda a posição atual do cursor
    fseek(file, 0, SEEK_END); // Vai até o final do arquivo
    if (ftell(file) == 0) { // Verifica se está vazio
        fprintf(file, "%s\n", header); // Escreve o cabeçalho
    }
    fseek(file, current_pos, SEEK_SET); // Retorna à posição original
}



// Função para imprimir o array de strings (artist_ids) em formato CSV
void print_string_array_to_csv(FILE *file, GArray* array) {
    for(guint i = 0; i < array->len; i++) {
        char* artist_id = g_array_index(array, char*, i);
        fprintf(file, "\"%s\"", artist_id);
        if (i < array->len - 1) {
            fprintf(file, ",");
        }
    }
}

//Função que junta os ids das músicas gostadas de um utilizador
char *join_liked_songs(GArray *liked_songs_id) {
    GString *joined = g_string_new("");  // Cria uma nova GString para armazenar o resultado

    for (guint i = 0; i < liked_songs_id->len; i++) {
        char *song_id = g_array_index(liked_songs_id, char*, i); // Corrigido para incluir o tipo char*
        g_string_append(joined, song_id);  // Adiciona à GString
        if (i < liked_songs_id->len - 1) {
            g_string_append(joined, ",");  // Adiciona uma vírgula entre os IDs
        }
    }

    return g_string_free(joined, FALSE);  // Retorna a string resultante
}

// Função para construir o caminho completo para os arquivos CSV
char* build_file_path(const char* folder_path, const char* file_name) {
    size_t path_len = strlen(folder_path) + strlen(file_name) + 2; // 1 para '/', 1 para o '\0'
    char* full_path = (char*)malloc(path_len * sizeof(char));

    if (full_path == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o caminho do arquivo.\n");
        exit(1);
    }

    snprintf(full_path, path_len, "%s/%s", folder_path, file_name);

    return full_path;
}


// Função para remover aspas no início e no fim, e aspas duplicadas dentro da string
char* remover_aspa(const char* str) {
    // Cria uma cópia da string original para edição
    char* editable_str = g_strdup(str);
    size_t len = strlen(editable_str);
    
    // Verifica por aspas no início e no fim
    if (len >= 2 && editable_str[0] == '"' && editable_str[len - 1] == '"') {
        editable_str[len - 1] = '\0';  // Remove a aspa final
        editable_str++;                // Avança o ponteiro para ignorar a aspa inicial
    }
    
    // Remove aspas que possam estar no meio do valor
    char* ptr = editable_str;
    while ((ptr = strchr(ptr, '"')) != NULL) {
        memmove(ptr, ptr + 1, strlen(ptr));
    }

    return g_strdup(editable_str);  // Retorna uma nova cópia da string ajustada
}

//Função que verifica se um país é permitido
int pais_permitido(const char* pais_artista, GHashTable* paises_permitidos) {
    GHashTableIter iter;
    gpointer key;
    g_hash_table_iter_init(&iter, paises_permitidos);
    
    while (g_hash_table_iter_next(&iter, &key, NULL)) {
        if (g_ascii_strcasecmp(pais_artista, (char*)key) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função para remover aspas no início e no fim de uma string
char* remover_aspas_dup(const char* str) {
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
        return strndup(str + 1, len - 2); 
    } else {
        return strdup(str); 
    }
}


// Função para converter segundos em uma string no formato HH:MM:SS
char *utils_segundos_para_string(int seg){
    int horas = seg / 3600;
    int minutos = (seg % 3600) / 60;
    int segundos = seg % 60;
    
    char *resultado = (char *)malloc(9 * sizeof(char));
    if (resultado == NULL) return NULL;

    snprintf(resultado, 9, "%02d:%02d:%02d", horas, minutos, segundos);
    return resultado;
}

//Processa uma string, extraindo substrings entre aspas simples e armazenando-as nm GArray.
GArray* funcao(const char* s) {

    if (s == NULL)
        return NULL;

    GArray* lista = g_array_new(FALSE, FALSE, sizeof(char*));
    int flag = -1;
    //   -1 -> ignorar  |  1 -> copiar

    int i = 0;
    char string[1024];
    string[0] = '\0'; // Faz com que a string seja vazia

    // Enquanto houver chars
    while (*s != '\0') {
        
        // Se o char for '
        if (*s == '\'') {

            flag *= -1; // Troca a flag

            if (i > 0) { // Já houve coisas copiadas para a string

                string[i] = '\0'; // Para termos uma string correta
                char aux[2024];
                aux[0] = '\0';
                sprintf(aux, "\"%s\"", string);

                // Armazena strdup(aux) em uma variável temporária antes de passar para g_array_append_val
                char *duplicated_string = strdup(aux);
                g_array_append_val(lista, duplicated_string); // Adicionamos ao array dinâmico
                
                string[0] = '\0'; // Reseta a string
                i = 0;
            }

        } else {

            // Devemos copiar chars
            if (flag == 1) {
                string[i] = *s;
                i++;
            }
        }

        s++; // Avança na string-argumento
    }

    return lista;
}


// Função para converter um género em um inteiro
int genero_to_int(char* genero) {
    char* genero_sem_aspas = remover_aspas_dup(genero);
    int result = -1; 

    if (strcmp(genero_sem_aspas, "Electronic") == 0) {
        result = 0;
    } else if (strcmp(genero_sem_aspas, "Hip Hop") == 0) {
        result = 1;
    } else if (strcmp(genero_sem_aspas, "Metal") == 0) {
        result = 2;
    } else if (strcmp(genero_sem_aspas, "Country") == 0) {
        result = 3;
    } else if (strcmp(genero_sem_aspas, "Blues") == 0) {
        result = 4;
    } else if (strcmp(genero_sem_aspas, "Reggae") == 0) {
        result = 5;
    } else if (strcmp(genero_sem_aspas, "Classical") == 0) {
        result = 6;
    } else if (strcmp(genero_sem_aspas, "Jazz") == 0) {
        result = 7;
    } else if (strcmp(genero_sem_aspas, "Pop") == 0) {
        result = 8;
    } else if (strcmp(genero_sem_aspas, "Rock") == 0) {
        result = 9;
    }

    free(genero_sem_aspas); 
    return result;
}


//Funçao para limpar o id de um artista
char *clean_artist_id(const char *input) {

    size_t id_len = strlen(input) - 6;
    char *result = malloc(id_len + 3); 


    result[0] = '"';

    strncpy(result + 1, input + 3, id_len);

    result[id_len + 1] = '"';
    result[id_len + 2] = '\0';

    return result;
}

//Função para processar uma string
char** process_string(const char* input) {
    int len = strlen(input);
    char* temp = strdup(input);
    int count = 1;

    if (temp[0] == '"') memmove(temp, temp + 1, len--);
    if (temp[len-1] == '"') temp[--len] = '\0';
    if (temp[0] == '[') memmove(temp, temp + 1, len--);
    if (temp[len-1] == ']') temp[--len] = '\0';

    for (int i = 0; i < len; i++) {
        if (temp[i] == ',') count++;
    }
    
    char** result = malloc(count * sizeof(char*));
    
    char* token = strtok(temp, ",");
    int i = 0;
    while (token != NULL) {
        while (*token == ' ' || *token == '\'') token++;
        int token_len = strlen(token);
        while (token_len > 0 && (token[token_len-1] == ' ' || token[token_len-1] == '\'')) {
            token[--token_len] = '\0';
        }
        
        result[i] = malloc(token_len + 3);
        sprintf(result[i], "\"%s\"", token);
        
        token = strtok(NULL, ",");
        i++;
    }
    
    free(temp);
    return result;
}