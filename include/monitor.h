#ifndef MONITOR_H
#define MONITOR_H

#include <mqueue.h> // Biblioteca para colas POSIX

#include "metadata.h"
#include "worker.h"
#include "pipes.h"
#include "ipc.h"
#include "logger.h"

// Comparar dos listas de metadatos obtenidas por el scanner recursivo
void detect_changes(FileMetadata *old_list, int old_count, FileMetadata *new_list, int new_count,int pipe_fd[2], mqd_t logger);

// Iniciar el monitoreo del directorio
void start_monitor(const char *origin);

#endif
