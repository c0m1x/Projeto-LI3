# Compilador e flags
CC = gcc
#-flto
CFLAGS = -O2 -g -fprefetch-loop-arrays -fomit-frame-pointer -Wall -Wno-format-truncation -march=native -funroll-all-loops -Iinclude -Iinclude/Catalogos -Iinclude/Queries -Iinclude/Structures -Iinclude/Utilities -Iinclude/Parser -Iinclude/Validations -Iinclude/recomendador `pkg-config --cflags glib-2.0`
LDFLAGS = `pkg-config --libs glib-2.0` -lm -flto

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
RESULT_DIR = resultados
RESULT_ESP_DIR = resultados-esperados

# Arquivos de origem específicos para cada executável
SRC_MAIN = $(filter-out $(SRC_DIR)/programa-testes.c, $(filter-out $(SRC_DIR)/programa-interativo.c , $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)))
SRC_TEST = $(filter-out $(SRC_DIR)/programa-principal.c, $(filter-out $(SRC_DIR)/programa-interativo.c , $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)))
SRC_INT =  $(filter-out $(SRC_DIR)/programa-principal.c, $(filter-out $(SRC_DIR)/programa-testes.c , $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)))

# Objetos específicos para cada executável
OBJ_MAIN = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_TEST = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_TEST))
OBJ_INT = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_INT))

# Nomes dos executáveis finais
TARGET = programa-principal
TEST_TARGET = programa-testes
INT_TARGET = programa-interativo

EXTERNAL_OBJ = $(OBJ_DIR)/recomendador-linux-x86_64.o

# Regra padrão para compilar o programa principal e o programa de testes
all: $(TARGET) $(TEST_TARGET) $(INT_TARGET)

# Regras para compilar os executáveis
$(TARGET): $(OBJ_MAIN) $(EXTERNAL_OBJ)
	$(CC) $(CFLAGS) $(OBJ_MAIN) $(EXTERNAL_OBJ) $(LDFLAGS) -o $@

# Regra para compilar o programa de testes
$(TEST_TARGET): $(OBJ_TEST)
	$(CC) $(CFLAGS) $(OBJ_TEST) $(EXTERNAL_OBJ) $(LDFLAGS) -o $@

#Regra para compilar o programa interativo
$(INT_TARGET): $(OBJ_INT)
	$(CC) $(CFLAGS) $(OBJ_INT) $(EXTERNAL_OBJ) $(LDFLAGS) -o $@

# Regras para compilar cada arquivo .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos compilados
clean:
	# Remove todos os arquivos .o exceto recomendador-linux-x86_64.o
	find $(OBJ_DIR) -type f -name '*.o' ! -name 'recomendador-linux-x86_64.o' -exec rm -f {} +
	rm -rf $(TARGET) $(TEST_TARGET) $(INT_TARGET)
	rm -rf $(RESULT_DIR)/*.csv
	rm -rf $(RESULT_DIR)/*.txt



.PHONY: all clean
