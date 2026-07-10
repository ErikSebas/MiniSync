#ifndef METADATA_H
#define METADATA_H

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>


#define MAX_PATH PATH_MAX // Longitud maxima de una ruta en linux: 4096
			  
typedef enum { // Tipos de eventos detectado en el monitor
    CREATE,
    MODIFY,
    DELETE
} EventType;

typedef struct { // Metadatos de un archivo
    char path[MAX_PATH]; 	// Ruta
    unsigned long inode; 	// Numero i-nodo
    long long size; 	 	// Tamaño
    unsigned int permissions;   // Permisos (No puede ser negativo)
    unsigned int user_id;       // Id del propietario (No puede ser negativo)
    unsigned int group_id;      // Id del grupo del propietario (No puede ser negativo)
    unsigned int hard_links;    // Numero de enlaces fisicos
    // Marcas de tiempo
    time_t acces_time;  	// Ultimo acceso             
    time_t modify_time; 	// Ultima modifcación
    time_t change_time;		// Ultimo cambio de metadatos
    int is_directory;		// 1 si es directorio
    int is_symlink;		// 1 si es directorio
} FileMetadata;

typedef struct { // Evento que el monitor enviara al worker
    EventType type;
    FileMetadata file;
} FileEvent;

int get_metadata(const char *path, FileMetadata *meta);

#endif 
