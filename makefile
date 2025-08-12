CC = gcc
CFLAGS = -Werror -Wall -Iinclude -std=c11
LDFLAGS = -Llib
LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32

DIR = src
PROG = main.c glad.c
SRC = $(addprefix $(DIR)/, $(PROG))
BIN = program
OUT = -o

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) $(OUT) $(BIN) $(LDFLAGS) $(LIBS)

run: $(BIN)
	@echo "Executando e Compilando..."
	@./$(BIN)

clean:
	@echo "Limpando"
	@rm -f $(BIN)

.PHONY: all run clean
