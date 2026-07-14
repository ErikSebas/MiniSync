#include "logger.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

//---Funciones del logger---

// Crea o abre la cola de mensajes
mqd_t create_logger() {

    mq_unlink(LOGGER_QUEUE);

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(LogMessage);
    attr.mq_curmsgs = 0;
    mqd_t logger;

    logger = mq_open(LOGGER_QUEUE,O_CREAT | O_RDWR,0644,&attr);

    if (logger == (mqd_t)-1) {
        perror("Error al crear la cola de mensajes");
    }

    return logger;
}

// Cierra la cola de mensajes
void close_logger(mqd_t logger) {
    mq_close(logger);
}

// Elimina la cola de mensajes
void unlink_logger() {
    mq_unlink(LOGGER_QUEUE);
}

// Envía un mensaje al logger
int send_log(mqd_t logger, LogMessage *msg) {

    if (mq_send(logger,
                (const char *)msg,
                sizeof(LogMessage),
                0) == -1) {

        perror("Error al enviar el mensaje");
        return -1;
    }
    return 0;
}

// Recibe un mensaje y lo escribe en el archivo de log
void logger_process(mqd_t logger, const char *file) {

    FILE *log = fopen(file, "a");

    if (log == NULL) {
        perror("Error al abrir el archivo de log");
        return;
    }

    LogMessage msg;

    if (mq_receive(logger,(char *)&msg,sizeof(LogMessage),NULL) == -1) {

        perror("Error al recibir el mensaje");
        fclose(log);
        return;
    }

    fprintf(log, "%s\n", msg.text);
    fflush(log);
    fclose(log);
}
