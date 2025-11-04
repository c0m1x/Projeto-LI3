#ifndef HISTORIC_H
#define HISTORIC_H

#include <string.h>

/**
 * @brief Valida as informações de um historico.
 * 
 * Verifica se a plataforma de um histórico é válida.
 * 
 * @param platform plataforma.
 * @return 1 se válido, 0 caso contrário.
 */
int is_history_valid(const char* platform);

#endif