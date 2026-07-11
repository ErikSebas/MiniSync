#include "logger.h"

#include <stdio.h>
#include <string.h>

int main() {

    mqd_t logger;

    LogMessage msg;

    // Crear la cola
    logger = create_logger();

    // Mensaje de prueba
    strcpy(msg.text, "CREATE prueba.txt");

    // Enviar mensaje
    send_log(logger, &msg);

    // Recibir el mensaje y escribirlo en el log
    logger_process(logger, "minisync.log");

    // Cerrar y eliminar la cola
    close_logger(logger);
    unlink_logger();

    printf("Prueba del logger finalizada.\n");

    return 0;
}
