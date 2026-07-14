#include "monitor.h"
#include "scanner.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//----Funciones auxiliares----

// Crea los procesos worker
static void create_workers(int workers, int pipe_fd[2], Stats *stats, sem_t *sem) {
    pid_t pid;
    FileEvent event;

    for (int i = 0; i < workers; i++) {

        pid = fork();

        if (pid < 0) {
            perror("Error al crear el worker");
            return;
        }

        // Proceso hijo (Worker)
        if (pid == 0) {

            close(pipe_fd[WRITE_END]);

            printf("Worker %d iniciado (PID: %d)\n", i + 1, getpid());

            while (1) {

                if (receive_event(pipe_fd[READ_END], &event) == 0) {
                    process_event(&event, stats, sem);
                }
            }
        }
    }

    // El monitor solo escribe en el pipe
    close(pipe_fd[READ_END]);
}


// Envía un mensaje al logger
static void log_event(mqd_t logger, const char *message) {
    LogMessage log;
    strncpy(log.text, message, sizeof(log.text) - 1);
    log.text[sizeof(log.text) - 1] = '\0';

    send_log(logger, &log);
}

// Inicia el monitoreo del directorio
void start_monitor(const char *origin,mqd_t logger) {

    int pipe_fd[2];

    Stats *stats;
    sem_t *sem;

    FileMetadata *old_list = NULL;
    FileMetadata *new_list = NULL;

    int old_count = 0;
    int new_count = 0;

    int old_capacity = 100;
    int new_capacity = 100;

    old_list = malloc(old_capacity * sizeof(FileMetadata));
    new_list = malloc(new_capacity * sizeof(FileMetadata));

    // Crear recursos del sistema
    create_pipe(pipe_fd);
    stats = create_shared_memory();
    sem = create_semaphore();

    // Crear procesos worker
    create_workers(2, pipe_fd, stats, sem);

    while (1) {

        new_count = 0; // Inicializara contador de archivos encontrados
        recursive_scan(origin, &new_list, &new_count, &new_capacity); // Escanear el directorio

        detect_changes(old_list, old_count, new_list, new_count, pipe_fd, logger); // Compara cambios detectados
        
	// Preparar la siguiente comparación
        free(old_list);
        old_list = new_list;
        old_count = new_count;
        new_list = malloc(new_capacity * sizeof(FileMetadata));

        sleep(5); // Detecta cambios cada 5 segundos 
    }
	

    // Liberar recursos (si el monitoreo termina)
	close_pipe(pipe_fd);

	close_logger(logger);
	unlink_logger();

	close_semaphore(sem);
	unlink_semaphore();

	close_shared_memory(stats);
	unlink_shared_memory();

	free(old_list);
	free(new_list);
  }

// Compara dos listas de metadatos obtenidas por el scanner recursivo
void detect_changes(FileMetadata *old_list, int old_count,FileMetadata *new_list, int new_count,int pipe_fd[2], mqd_t logger) {

    FileEvent event;
    char message[MAX_LOG_SIZE];

    // Detectar archivos nuevos y modificados
    for (int i = 0; i < new_count; i++) {
        int found = 0;
        int modified = 0;

        for (int j = 0; j < old_count; j++) {


            if (strcmp(new_list[i].path, old_list[j].path) == 0) {
                found = 1;
                if (new_list[i].modify_time != old_list[j].modify_time ||
                    new_list[i].size != old_list[j].size) {
                    modified = 1;
                }
                break;
            }
        }

        if (!found) {
            event.type = CREATE;
            event.file = new_list[i];
            send_event(pipe_fd[WRITE_END], &event);
            snprintf(message, sizeof(message),"[CREATE] %s", new_list[i].path);

            log_event(logger, message);
        }

        else if (modified) {
            event.type = MODIFY;
            event.file = new_list[i];
            send_event(pipe_fd[WRITE_END], &event);

            snprintf(message, sizeof(message),"[MODIFY] %s", new_list[i].path);
            log_event(logger, message);
        }

    }

    // Detectar archivos eliminados
    for (int j = 0; j < old_count; j++) {
        int still_exists = 0;
        for (int i = 0; i < new_count; i++) {
            if (strcmp(old_list[j].path, new_list[i].path) == 0 ){
                still_exists = 1;
                break;
		 }
        }

        if (!still_exists) {
            event.type = DELETE;
            event.file = old_list[j];
            send_event(pipe_fd[WRITE_END], &event);
            snprintf(message, sizeof(message),"[DELETE] %s", old_list[j].path);
            log_event(logger, message);
        }
    }
}


