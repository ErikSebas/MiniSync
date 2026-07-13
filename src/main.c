#include "daemon.h"
#include "monitor.h"

#include <stdio.h>

int main() {

    printf("Iniciando MiniSync...\n");

    // Pendiente Convertir el proceso en un daemon(segundo plano)
 

    // Iniciar el monitoreo del directorio
    start_monitor("origin");

    return 0;
}
