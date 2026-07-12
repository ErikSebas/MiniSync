#ifndef MONITOR_H
#define MONITOR_H

#include "metadata.h"

// Comparar dos listas de metadatos obtenidas por el scanner recursivo
void detect_changes(FileMetadata *old_list, int old_count, FileMetadata *new_list, int new_count);

// Iniciar el monitoreo del directorio
void start_monitor(const char *origin, const char *backup);

#endif
