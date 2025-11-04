#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <glib.h>

typedef struct history History;
typedef struct user_histories UserHistories;
typedef struct year_histories YearHistories;

/**
 * @brief Devolve a hashtable com todos os users daquele ano.
 * @param year_histories estrutura do ano.
 * @param user_id O ano.
 * @return Hashtable de users daquele ano.
 */
UserHistories* get_users_table_by_year(YearHistories* year_histories, char* user_id);

/**
 * @brief Devolve a hashtable com todos os historicos daquele user.
 * @param year_histories estrutura do user.
 * @param user_id O user_id.
 * @return Hashtable de historico daquele user.
 */
GHashTable* get_user_histories_table(UserHistories* user_histories);

/**
 * @brief Faz malloc de YearHistories.
 * @return YearHistories.
 */
YearHistories* malloc_year(char* user_id, History* history);

/**
 * @brief Adiciona um user ao ano.
 * @param year ano.
 * @param user_id user.
 * @param history historico.
 */
void set_user_in_year(YearHistories* year, char* user_id, History* history);

/**
 * @brief Obtém o ID de um histórico.
 * @param history O histórico.
 * @return O ID do histórico.
 */
char* get_history_id(const History* history);

/**
 * @brief Obtém o mês de um histórico.
 * @param history O histórico.
 * @return O mês do histórico.
 */
char* get_history_month(const History* history);

/**
 * @brief Obtém o dia de um histórico.
 * @param history O histórico.
 * @return O dia do histórico.
 */
char* get_history_day(const History* history);

/**
 * @brief Obtém a hora de um histórico.
 * @param history O histórico.
 * @return A hora do histórico.
 */
char* get_history_hour(const History* history);

/**
 * @brief Obtém o ID da música de um histórico.
 * @param history O histórico.
 * @return O ID da música.
 */
char* get_history_music_id(const History* history);

/**
 * @brief Obtém o timestamp de um histórico.
 * @param history O histórico.
 * @return O timestamp do histórico.
 */
char* get_history_timestamp(const History* history);

/**
 * @brief Obtém a duração de um histórico.
 * @param history O histórico.
 * @return A duração do histórico.
 */
char* get_history_duration(const History* history);

/**
 * @brief Libera a memória de um histórico de usuário.
 * @param uh O histórico de usuário.
 */
void free_user_histories(UserHistories* uh);

/**
 * @brief Função auxiliar para liberar a memória de um histórico de usuário.
 * @param user O histórico de usuário.
 */
void free_user_histories_void(gpointer user);

/**
 * @brief Libera a memória de um histórico de ano.
 * @param yh O histórico de ano.
 */
void free_year_histories(YearHistories* yh);

/**
 * @brief Função auxiliar para liberar a memória de um histórico de ano.
 * @param year O histórico de ano.
 */
void free_year_histories_void(gpointer year);

/**
 * @brief Liberta a memória de um histórico.
 * @param history O histórico.
 */
void free_history(History* history);

/**
 * @brief Função auxiliar para liberar a memória de um histórico.
 * @param history O histórico.
 */
void free_history_void(gpointer history);

/**
 * @brief Insere um histórico em uma tabela de hash.
 * @param table A tabela de hash.
 * @param history O histórico a ser inserido.
 */
void insertHistory(GHashTable* table, History* history);

/**
 * @brief Busca um histórico em uma tabela de hash pelo ID.
 * @param table A tabela de hash.
 * @param id O ID do histórico a ser buscado.
 * @return O histórico encontrado, ou NULL se não encontrado.
 */
History* findHistory(GHashTable* table, const char* id);

/**
 * @brief Clona um histórico.
 * @param history O histórico a ser clonado.
 * @return Uma cópia do histórico, ou NULL se falhar.
 */
History* cloneHistory(const History* history);

/**
 * @brief Valida os tokens de um histórico.
 * @param tokens Os tokens a serem validados.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_tokens_history(char** tokens);

/**
 * @brief Completa um histórico a partir de uma linha de texto.
 * @param line A linha de texto.
 * @param tokens Os tokens extraídos da linha.
 * @return O histórico completo, ou NULL em caso de erro.
 */
History* completa_History(char* line, char** tokens);

/**
 * @brief Cria uma tabela de usuários no contexto de um ano.
 * @param year A estrutura do ano.
 */
void create_users_table(YearHistories* year);

/**
 * @brief Cria uma tabela de históricos no contexto de um ano.
 * @param year A estrutura do ano.
 */
void create_history_table(YearHistories* year);

#endif // HISTORY_H
