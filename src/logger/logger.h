#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <windows.h>

// Global log file pointer and mutex for thread safety.
extern FILE *logFile;
extern HANDLE logMutex;

void init_logger();
void write_log(const char *message);
void close_logger();

#endif // LOGGER_H
