#ifndef CATALOGO_ALBUM_H
#define CATALOGO_ALBUM_H

#include <glib.h>
#include "album.h"

// Estrutura do catálogo de álbuns
typedef struct album_catalog Album_catalog;

// Funções públicas para o catálogo de álbuns


/**
 * @brief Libera a memória associada ao catálogo de álbuns.
 * 
 * @param catalogo Ponteiro para o catálogo de álbuns a ser liberado.
 */
void free_album_catalog(Album_catalog* catalogo);


/**
 * @brief Verifica se um álbum com o ID especificado está no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de álbuns.
 * @param album_id ID do álbum a ser verificado.
 * @return TRUE se o álbum estiver no catálogo, FALSE caso contrário.
 */
gboolean album_Contains(Album_catalog* catalogo, char* album_id);

/**
 * @brief Constrói um álbum e insere no catálogo a partir de uma linha de dados.
 * 
 * @param line Linha contendo os dados do álbum.
 * @param tokens Vetor de tokens extraídos da linha.
 * @param catalogo Ponteiro para o catálogo de álbuns onde o álbum será inserido.
 */
void buildAlbum(char* line, char** tokens, Album_catalog* catalogo, void* artist_catalogo);

/**
 * @brief Função wrapper para construir um álbum a partir de uma linha e inserir em várias estruturas.
 * 
 * @param line Linha contendo os dados do álbum.
 * @param tokens Vetor de tokens extraídos da linha.
 * @param structure1 Ponteiro para a primeira estrutura.
 * @param structure2 Ponteiro para a segunda estrutura.
 * @param structure3 Ponteiro para a terceira estrutura.
 * @param structure4 Ponteiro para a quarta estrutura.
 * @param structure5 Ponteiro para a quinta estrutura.
 */
void buildAlbum_wrapper(char* line, char** tokens, void* structure1, void* structure2, void* structure3, void* structure4, void* structure5);

/**
 * @brief Lê álbuns de um arquivo e constrói um catálogo.
 * 
 * @param path Caminho para o arquivo contendo os dados dos álbuns.
 * @return Ponteiro para o catálogo de álbuns criado.
 */
Album_catalog* albumToCatalog(char* path, void* structure1);

#endif // CATALOGO_ALBUM_H
