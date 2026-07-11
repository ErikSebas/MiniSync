#ifndef MONITOR_H
#define MONITOR_H

#include "metadata.h"
#include "worker.h"
#include "pipes.h"
#include "ipc.h"
#include "logger.h"

// Comparar dos listas de metadatos obtenidas por el scanner recursivo
void detect_changes(FileMetadata *old_list, int old_count,FileMetadata *new_list, int new_count);

// Iniciar el monitoreo del directorio
void start_monitor(const char *origin, const char *backup);

// Crear los procesos worker mediante fork()
void create_workers(int workers);

// Envíar un evento al worker mediante pipes
void send_event(FileEvent *event);

// Registrar un evento en el logger
void log_event(mqd_t logger, const char *message);

#endif
