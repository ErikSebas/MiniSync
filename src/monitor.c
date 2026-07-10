#include "monitor.h"
#include <stdio.h>
#include <string.h>

void detect_changes(FileMetadata *old_list, int old_count, FileMetadata *new_list, int new_count) {
    printf("\n----- [Monitor de archivos]-----\n");
    printf("\n----Iniciando análisis de cambios----\n");

    // 1.Detectar archivos nuevos y modificados


    for(int i = 0; i < new_count;i++){  // Recorrer lista nueva
    	int found = 0;
	int modified = 0;
    
    	for(int j = 0; j < old_count;j++){ // Buscar si archivo actual existe en nueva lista
		if(new_list[i].inode == old_list[j].inode){ // Comparar usando el inodo
		  found = 1; // El archivo ya existia

		  	// Si el archivo ya existia, revisar si tuvo cambios
			if(new_list[i].modify_time != old_list[j].modify_time || new_list[i].size != old_list[j].size) {
			  modified = 1;
			}
			break;
		}
	}

	// Evaluar resultados del primer barrido
	if (!found) {
            printf("[NEW] Archivo nuevo detectado %s (Inodo: %lu, Tamaño: %lld bytes)\n",  // Archivo nuevo
                   new_list[i].path, new_list[i].inode, new_list[i].size);
	   } else if (modified) {
            printf("[MODIFY] Archivo modificado %s )\n", new_list[i].path);  // Archivo modificado
	   }
	

    // 2.Detectar archivos eliminados 

    // Recorrer lista vieja
    for (int j = 0; j < old_count; j++) {
        int still_exists = 0;

        // Buscamos si el archivo del pasado sigue vivo en la lista NUEVA
        for (int i = 0; i < new_count; i++) {
            if (old_list[j].inode == new_list[i].inode) {
                still_exists = 1; // Sigue ahí
                break; // Rompemos bucle interno
            }
        }

     // Si en la lista nueva no aparece el inodo viejo
     if (!still_exists) {
            printf("[DELETED] Archivo eliminado %s (Ya no existe en el directorio local)\n", old_list[j].path);
        }
    }

    printf("--- [Monitor de archivos finalizado] ---\n\n");
}
