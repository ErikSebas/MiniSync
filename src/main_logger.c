#include "logger.h"
#include <stdio.h>

int main(){

    printf("Logger iniciado\n");

    mqd_t logger = create_logger();

    if(logger == (mqd_t)-1){
        return 1;
    }

    logger_process(logger, "log.txt");

    return 0;
}
