CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/parser.c src/tablas.c src/ast.c

TARGET = analizador

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC) include/parser.h include/tablas.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./pruebas/probar_analizador.sh

clean:
	rm -f $(TARGET)
