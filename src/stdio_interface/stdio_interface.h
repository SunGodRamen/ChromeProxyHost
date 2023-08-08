#ifndef STDIO_INTERFACE_H
#define STDIO_INTERFACE_H

#include <stdint.h>

#define BUFFER_SIZE 1024

// Write data to the stdio interface
void write_stdio(const char *jsonOutput);

// Read data from the stdio interface
int read_stdio(char *output, uint32_t bufferSize, uint32_t *outputLength);

#endif
