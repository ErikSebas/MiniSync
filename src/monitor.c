#include "monitor.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//----Funciones auxiliares----

// Crea los procesos worker
static void create_workers(int workers) {
    pid_t pid;

    for (int i = 0; i < workers; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error al crear el worker");
            return;
        }

        if (pid == 0) {
            printf("Worker %d iniciado (PID: %d)\n", i + 1, getpid());

            while (1) {
                pause(); // Pendiente hacer lo que sigue del worker
            }
        }
    }
}
