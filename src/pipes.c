#include "pipes.h"

#include <stdio.h>
#include <unistd.h>

//---Funciones de los pipes---

// Crea un pipe
int create_pipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Error al crear el pipe");
        return -1;
    }

    return 0;
}

// Envía un evento al worker
int send_event(int write_fd, FileEvent *event) {
    ssize_t bytes_written;
    bytes_written = write(write_fd, event, sizeof(FileEvent));

    if (bytes_written != sizeof(FileEvent)) {
        perror("Error al escribir en el pipe");
        return -1;
    }

    return 0;
}

// Recibe un evento enviado por el monitor
int receive_event(int read_fd, FileEvent *event) {
    ssize_t bytes_read;
    bytes_read = read(read_fd, event, sizeof(FileEvent));

    if (bytes_read != sizeof(FileEvent)) {
        perror("Error al leer del pipe");
        return -1;
    }

    return 0;
}

// Cierra ambos extremos del pipe
void close_pipe(int pipe_fd[2]) {

    close(pipe_fd[READ_END]);
    close(pipe_fd[WRITE_END]);
}
