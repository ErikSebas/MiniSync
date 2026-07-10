#include "metadata.h"
#include <string.h>

// Función privada: Solo se ve y se usa dentro de metadata.c
static void fill_fields(FileMetadata *meta, struct stat *chosen, int is_lnk) {
    meta->is_symlink   = is_lnk ? 1 : 0; // Indica si es enlace simbolico
    meta->inode        = chosen->st_ino; 
    meta->size         = chosen->st_size;
    meta->permissions  = chosen->st_mode;
    meta->user_id      = chosen->st_uid;
    meta->group_id     = chosen->st_gid;
    meta->hard_links   = chosen->st_nlink;
    meta->acces_time   = chosen->st_atime;
    meta->modify_time  = chosen->st_mtime;
    meta->change_time  = chosen->st_ctime;
    meta->is_directory = S_ISDIR(chosen->st_mode) ? 1 : 0;
}

// Obtiene los metadatos usando stat y lstat
int get_metadata(const char *path, FileMetadata *meta) {
    struct stat target_stat; // Informacion del archivo real
    struct stat link_stat;   // Informacion del enlace simbolico		

    if (lstat(path, &link_stat) == -1){ // lstat(): Obtiene informacion del archivo sin seguir enlaces simbolicos
	    return -1;
    }

    int stat_result = stat(path, &target_stat); // stat(): Sigue el enlace simbolico al archivo real

    strncpy(meta->path, path, sizeof(meta->path) - 1);
    meta->path[sizeof(meta->path) - 1] = '\0';

    // Si stat() funciona leer archivo real, caso contrario usar el enlace simbolico
    struct stat *chosen = (stat_result != -1) ? &target_stat : &link_stat;

    fill_fields(meta, chosen, S_ISLNK(link_stat.st_mode)); // Rellenar la estrucutra con los datos seleccionados
	
    return 0;
}
