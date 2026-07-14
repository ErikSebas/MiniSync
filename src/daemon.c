#include "daemon.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void make_daemon() {

    pid_t pid;

    // Primer fork
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Termina el proceso padre
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Crear una nueva sesión
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // Segundo fork
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Termina el primer hijo
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Reiniciar máscara de permisos
    umask(0);

    /*
     * No cambiamos el directorio de trabajo para que el daemon
     * siga utilizando las rutas relativas del proyecto:
     * origin/, backup/ y minisync.log
     */

    /*
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }
    */

    // Cerrar descriptores estándar
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirigir stdin, stdout y stderr a /dev/null
    if (open("/dev/null", O_RDONLY) == -1) {
        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) == -1) {
        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) == -1) {
        exit(EXIT_FAILURE);
    }
}
