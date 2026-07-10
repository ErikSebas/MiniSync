#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void recursive_scan(const char *directory, FileMetadata **list, int *count, int *capacity) {
    DIR *dir = opendir(directory);
    struct dirent *entry;

    if (dir == NULL) {
         printf("Error: No se pudo encontrar el directorio %s\n", directory);
         return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        // Control de memoria dinámica con realloc
        if (*count >= *capacity) {
            int new_capacity = (*capacity) * 2;
            FileMetadata *temp = realloc(*list, new_capacity * sizeof(FileMetadata));
            if (temp == NULL) {
                printf("Error: No hay suficiente memoria RAM \n");
                closedir(dir);
                return;
            }
            *list = temp;
            *capacity = new_capacity;
            printf("[Memoria] Capacidad duplicada a %d elementos.\n", *capacity);
        }

        char full_route[MAX_PATH];
        snprintf(full_route, sizeof(full_route), "%s/%s", directory, entry->d_name);

        FileMetadata meta;

        // Esta función ahora la provee metadata.c de forma transparente
        if (get_metadata(full_route, &meta) == -1) continue;

        (*list)[*count] = meta;
        (*count)++;

        if (meta.is_directory == 1) {
            recursive_scan(full_route, list, count, capacity);
        } else {
            printf("Archivo Encontrado: %s\n", full_route);
        }
    }
    closedir(dir);
}
