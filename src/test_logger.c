#include "logger.h"
#include <stdio.h>
#include <fcntl.h>

int main() {

    mqd_t logger = mq_open(LOGGER_QUEUE, O_WRONLY);

    if (logger == (mqd_t)-1) {
        perror("Error al abrir la cola");
        return 1;
    }

    LogMessage msg;

    printf("Enviando mensaje...\n");

    snprintf(msg.text,
             MAX_LOG_SIZE,
             "Prueba del sistema de logging");

    send_log(logger, &msg);

    printf("Mensaje enviado\n");

    mq_close(logger);

    return 0;
}
