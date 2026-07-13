#ifndef WORKER_H
#define WORKER_H

#include "metadata.h"
#include "ipc.h" // Integracion de ipc con worker

//---Funciones del worker---
// Uso de open(), read(), write(), close() y fsync
int copy_file(const char *source, const char *destination,Stats *stats , sem_t *sem); 

int delete_file(const char *path, Stats *stats, sem_t *sem); 

void process_event(FileEvent *event, Stats *stats, sem_t *sem); // Procesa el evento realizado por el metadata

#endif
