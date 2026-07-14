#ifndef IPC_H
#define IPC_H

#include <sys/mman.h>   // mmap()
#include <sys/stat.h>   // Permisos
#include <fcntl.h>      // shm_open()
#include <semaphore.h>  // Semáforos POSIX
#include <unistd.h>
#include <stddef.h>

#define SHM_NAME "/minisync_stats"
#define SEM_NAME "/minisync_sem"

// Estructura de estadisticas compartidas
typedef struct {
    long files_copied;      
    long bytes_copied;     
    long errors;            
} Stats;

//---Memoria Compartida---
Stats *create_shared_memory();

// Libera el mapeo de memoria compartida
void close_shared_memory(Stats *stats);

// Elimina completamente la memoria compartida
void unlink_shared_memory();


//---Semaforos POSIX---
// Crea o Abre el semaforo
sem_t *create_semaphore();

// Cierra el semaforo
void close_semaphore(sem_t *sem);

// Elimina semaforo del sistema
void unlink_semaphore();

#endif
