#ifndef MONITOR_H
#define MONITOR_H

#include "metadata.h" 

void detect_changes(FileMetadata *old_list, int old_count, FileMetadata *new_list, int new_count);

#endif
