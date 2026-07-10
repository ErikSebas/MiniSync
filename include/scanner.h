#ifndef SCANNER_H
#define SCANNER_H

#include "metadata.h" 

void recursive_scan(const char *directory, FileMetadata **list, int *count, int *capacity);

#endif
