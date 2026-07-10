#include "worker.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// ----Funciones Auxiliares----

// Actualiza las estadísticas después de una copia exitosa
void update_copy_stats(Stats *stats, sem_t *sem, long bytes) {
    if (stats != NULL && sem != NULL) {
        sem_wait(sem);
        stats->files_copied++;
        stats->bytes_copied += bytes;
        sem_post(sem);
    }
}

// Actualiza el contador de errores
void update_error_stats(Stats *stats, sem_t *sem) {
    if (stats != NULL && sem != NULL) {
        sem_wait(sem);
        stats->errors++;
        sem_post(sem);
    }
}

// ----Funciones del Worker----

// ----Copiar un archivo al directorio backup----
int copy_file(const char *source, const char *destination,
              Stats *stats, sem_t *sem) {

    int source_fd;
    int destination_fd;
    char buffer[4096];
    ssize_t bytes_read;

    // Abrir archivo origen
    source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("Error al abrir el archivo origen");
        update_error_stats(stats, sem);
        return -1;
    }

    // Obtener tamaño del archivo
    off_t file_size = lseek(source_fd, 0, SEEK_END);

    if (file_size == -1) {
        perror("Error con lseek()");
        update_error_stats(stats, sem);
        close(source_fd);
        return -1;
    }

    // Regresar al inicio
    if (lseek(source_fd, 0, SEEK_SET) == -1) {
        perror("Error al regresar al inicio");
        update_error_stats(stats, sem);
        close(source_fd);
        return -1;
    }

    // Crear archivo destino
    destination_fd = open(destination,O_WRONLY | O_CREAT | O_TRUNC,0644);

    if (destination_fd == -1) {
        perror("Error al crear el archivo destino");
        update_error_stats(stats, sem);
        close(source_fd);
        return -1;
    }

    // Copia secuencial
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {

        if (write(destination_fd, buffer, bytes_read) != bytes_read) {
            perror("Error al escribir");
            update_error_stats(stats, sem);
            close(source_fd);
            close(destination_fd);

            return -1;
        }
    }

    if (bytes_read == -1) {

        perror("Error al leer");
        update_error_stats(stats, sem);
        close(source_fd);
        close(destination_fd);

        return -1;
    }

    // Forzar escritura física
    if (fdatasync(destination_fd) == -1)
        perror("Error con fdatasync()");

    if (fsync(destination_fd) == -1)
        perror("Error con fsync()");

    printf("Archivo copiado correctamente (%lld bytes)\n",
           (long long)file_size);

    // Actualizar estadísticas
    update_copy_stats(stats, sem, file_size);

    close(source_fd);
    close(destination_fd);

    return 0;
}

// ----Elimina un archivo del directorio backup----
int delete_file(const char *path,
                Stats *stats,
                sem_t *sem) {

    if (unlink(path) == -1) {

        perror("Error al eliminar el archivo");

        update_error_stats(stats, sem);

        return -1;
    }

    printf("Archivo eliminado correctamente: %s\n", path);

    return 0;
}

// Procesa un evento recibido desde el monitor
void process_event(FileEvent *event) {

    switch (event->type) {

        case CREATE:
            printf("[CREATE] %s\n", event->file.path);
            break;

        case MODIFY:
            printf("[MODIFY] %s\n", event->file.path);
            break;

        case DELETE:
            printf("[DELETE] %s\n", event->file.path);
            break;

        default:
            printf("Evento desconocido.\n");
            break;
    }
}
