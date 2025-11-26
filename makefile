# Compilador a ser utilizado
CC = gcc

# Flags (opções) de compilação:
# -Wall -Wextra: Habilita avisos para ajudar a achar bugs
# -ansi -pedantic: Garante que o código siga o padrão ANSI C estrito (requisito)
CFLAGS = -Wall -Wextra -ansi -pedantic

# Nome do executável final
TARGET = calc

# Arquivos fonte do projeto
SRCS = main.c intgg.c

# Regra padrão (o que acontece quando digita apenas 'make')
all: $(TARGET)

# Linkagem: Cria o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Limpeza: Remove arquivos temporários e o executável
clean:
	rm -f $(TARGET) *.o saida_*.txt