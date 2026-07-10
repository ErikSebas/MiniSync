#ifndef PIPES_H
#define PIPES_H

#include "metadata.h"

#define READ_END 0
#define WRITE_END 1

//----Funciones de los pipes----

// Crea un pipe entre el monitor y el worker.
int create_pipe(int pipe_fd[2]);

// Envía un evento al worker mediante el pipe.
int send_event(int write_fd, FileEvent *event);

// Recibe un evento enviado por el monitor.
int receive_event(int read_fd, FileEvent *event);

#endif





