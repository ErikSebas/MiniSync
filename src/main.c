#include "daemon.h"
#include "monitor.h"
#include "logger.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {

    // Pendiente: convertir en daemon
    make_daemon();
    printf("Monitor de archivos iniciado....");

    // Crear la cola de mensajes
    mqd_t logger = create_logger();

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso logger");
        return EXIT_FAILURE;
    }

    // Proceso hijo: Logger
    if (pid == 0) {

        logger_process(logger, "minisync.log");

        close_logger(logger);
        return EXIT_SUCCESS;
    }

    // Proceso padre: Monitor
    start_monitor("origin", logger);

    close_logger(logger);

    return EXIT_SUCCESS;
}
