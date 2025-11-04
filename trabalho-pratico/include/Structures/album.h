#ifndef ALBUM_H
#define ALBUM_H

#include <glib.h>

/**
 * @brief Estrutura que representa um álbum.
 */
typedef struct album Album;

/**
 * @brief Libera a memória associada a um álbum.
 * 
 * @param album Ponteiro para o álbum a ser liberado.
 */
void free_album(Album* album);

/**
 * @brief Libera a memória de um álbum, usado em operações de hash table.
 * 
 * @param album Ponteiro para o álbum a ser liberado.
 */
void free_album_void(gpointer album);

/**
 * @brief Procura um álbum na tabela hash pelo ID.
 * 
 * @param table Tabela hash contendo os álbuns.
 * @param id ID do álbum a ser encontrado.
 * @return Ponteiro para o álbum encontrado ou NULL se não existir.
 */
Album* findAlbum(GHashTable* table, const char* id);

/**
 * @brief Cria uma cópia de um álbum.
 * 
 * @param album Ponteiro para o álbum a ser copiado.
 * @return Ponteiro para o novo álbum criado. O chamador deve liberar a memória.
 */
Album* cloneAlbum(const Album* album);

/**
 * @brief Inicializa e preenche os campos de um álbum a partir de uma linha e tokens.
 * 
 * @param line Linha de entrada contendo os dados do álbum.
 * @param tokens Array de tokens extraídos da linha.
 * @return Ponteiro para o álbum inicializado. O chamador deve liberar a memória.
 */
Album* completa_Album(char* line, char** tokens);

#endif /* ALBUM_H */
