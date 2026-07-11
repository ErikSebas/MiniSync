#ifndef LOGGER_H
#define LOGGER_H

#include <mqueue.h>

#define LOGGER_QUEUE "/logger" // Nombre de la cola de mensajes POSIX
#define MAX_LOG_SIZE 128

// Mensaje que viajará por la cola
typedef struct {
    char text[MAX_LOG_SIZE];
} LogMessage;

// Crea o abre la cola de mensajes
mqd_t create_logger();

// Cierra la cola de mensajes
void close_logger(mqd_t logger);

// Elimina la cola de mensajes
void unlink_logger();

// Envia un mensaje al logger
int send_log(mqd_t logger, LogMessage *msg);

// Recibe mensajes y los guarda en el archivo de log
void logger_process(mqd_t logger,const char *file);

#endif
