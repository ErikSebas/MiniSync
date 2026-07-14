#include "ipc.h"

#include <stdio.h>
#include <unistd.h>

//----Memoria Compartida----

// Crea o abre la memoria compartida
Stats *create_shared_memory() {

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        perror("Error en shm_open()");
        return NULL;
    }

    // Asignar tamaño a la memoria compartida
    if (ftruncate(shm_fd, sizeof(Stats)) == -1) {
        perror("Error en ftruncate()");
        close(shm_fd);
        return NULL;
    }

    // Mapear la memoria compartida
    Stats *stats = mmap(NULL,
                        sizeof(Stats),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        shm_fd,
                        0);

    close(shm_fd);

    if (stats == MAP_FAILED) {
        perror("Error en mmap()");
        return NULL;
    }

    // Inicializar estadísticas
    stats->files_copied = 0;
    stats->bytes_copied = 0;
    stats->errors = 0;

    return stats;
}

// Libera el mapeo de memoria compartida
void close_shared_memory(Stats *stats) {

    if (munmap(stats, sizeof(Stats)) == -1) {
        perror("Error en munmap()");
    }
}

// Elimina la memoria compartida
void unlink_shared_memory() {

    if (shm_unlink(SHM_NAME) == -1) {
        perror("Error en shm_unlink()");
    }
}


//----Semáforos POSIX----

// Crea o abre el semáforo
sem_t *create_semaphore() {

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    if (sem == SEM_FAILED) {
        perror("Error en sem_open()");
        return NULL;
    }

    return sem;
}

// Cierra el semáforo
void close_semaphore(sem_t *sem) {

    if (sem_close(sem) == -1) {
        perror("Error en sem_close()");
    }
}

// Elimina el semáforo
void unlink_semaphore() {

    if (sem_unlink(SEM_NAME) == -1) {
        perror("Error en sem_unlink()");
    }
}
