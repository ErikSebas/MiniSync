# Copilador
CC = gcc

# Opciones
CFLAGS = -Wall -Iinclude

# Bibliotecas
LDFLAGS = -pthread -lrt

# Archivos fuentes
SRC = src/main.c \
      src/daemon.c \
      src/monitor.c \
      src/worker.c \
      src/scanner.c \
      src/metadata.c \
      src/ipc.c \
      src/pipes.c \
      src/logger.c

# Ejecutable 
TARGET = MiniSync

# Compilar 
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Limpiar 
clean:
	rm -f $(TARGET)
