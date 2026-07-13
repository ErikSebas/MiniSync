#include "daemon.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void make_daemon() {
    pid_t pid;

    // Clona el proceso
    pid = fork();
    if (pid < 0) {        
    exit(EXIT_FAILURE); // Si falla el fork, abortar
    }

    // Muere el proceso padre
    if (pid > 0){         
    exit(EXIT_SUCCESS); 
    }	

    // Independiza al hijo: Crea una nueva sesión y se convierte en el líder del grupo de procesos.
    if (setsid() < 0){
    exit(EXIT_FAILURE);
    }

    // Evita que el proceso vuelva a abrir la terminal.
    pid = fork();
    if (pid < 0){ 
    exit(EXIT_FAILURE);
    }

    // El primer hijo termina, el daemon no
    if (pid > 0){
    exit(EXIT_SUCCESS); 
    }

    // Reinicia la máscara de permisos del proceso.
    umask(0);

    // Evita que el demonio bloquee el directorio desde donde se ejecutó.
    if (chdir("/") < 0){
    exit(EXIT_FAILURE);
    }

    // Cierra los canales estandar heredados por la terminal.
    close(STDIN_FILENO);  
    close(STDOUT_FILENO); 
    close(STDERR_FILENO); 

    // Redirije los descriptores estandar, si alguna función del sistema intenta imprimir algo, no romperá el daemon.
    if (open("/dev/null", O_RDONLY) == -1)
    exit(EXIT_FAILURE);
    
    if (open("/dev/null", O_RDWR) == -1)
    exit(EXIT_FAILURE);
    
    if (open("/dev/null", O_RDWR) == -1)
    exit(EXIT_FAILURE);
}
