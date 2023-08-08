#include "logger.h"

FILE *logFile = NULL;
HANDLE logMutex = NULL;

void init_logger() {
    logFile = fopen("C:\\Users\\avons\\Code\\Chrome\\Chrome-Proxy\\myLog.txt", "a");
    if (logFile == NULL) {
        // Print the error to stderr
        fprintf(stderr, "Error: Unable to open log file for writing.\n");
        exit(-1);
    }
    logMutex = CreateMutex(NULL, FALSE, NULL);
    if (logMutex == NULL) {
        // Print the error to stderr
        fprintf(stderr, "Error: Unable to create mutex.\n");
        fclose(logFile);
        exit(-1);
    }
}

void write_log(const char *message) {
    WaitForSingleObject(logMutex, INFINITE);  // Wait for ownership of the mutex.
    if (fprintf(logFile, "%s\n", message) < 0) {
        // Print the error to stderr if there's an issue writing to the log
        fprintf(stderr, "Error: Unable to write to log file.\n");
    }
    fflush(logFile);
    ReleaseMutex(logMutex);  // Release ownership of the mutex.
}

void close_logger() {
    if (logFile) {
        fclose(logFile);
    }
    if (logMutex) {
        CloseHandle(logMutex);
    }
}
