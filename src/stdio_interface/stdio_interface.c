#include "stdio_interface.h"
#include "../logger/logger.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define BUFFER_SIZE 1024

void write_stdio(const char *input, uint32_t inputLength, int stdioMsgId) {

#ifdef _WIN32
    // Ensure the I/O mode is set to binary on Windows
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    char buffer[BUFFER_SIZE];
    // Format the message in JSON format with the ID and content
    uint32_t messageLength = snprintf(buffer, sizeof(buffer), "{\"id\":%d,\"content\":\"%s\"}", stdioMsgId, input);

    // Check for overflow
    if (messageLength >= sizeof(buffer)) {
        write_log("Error: Message too long to fit in buffer");
        return;
    }

    // Write the message length (first 4 bytes)
    fwrite(&messageLength, sizeof(messageLength), 1, stdout);
    
    // Write the message content
    fwrite(buffer, sizeof(char), messageLength, stdout);

    // Flush the output buffer
    fflush(stdout);
}

// Read data from the stdio interface and extract message content
int read_stdio(char *output, uint32_t bufferSize, uint32_t *outputLength) {
    
#ifdef _WIN32
    // Ensure the I/O mode is set to binary on Windows
    _setmode(_fileno(stdin), _O_BINARY);
#endif

    // Read the message length (first 4 bytes)
    if (fread(outputLength, sizeof(*outputLength), 1, stdin) != 1) {
        // Handle read error
        *outputLength = 0;
        output[0] = '\0';
        return -1;
    }

    // Read the message content
    if (fread(output, sizeof(char), *outputLength, stdin) != *outputLength) {
        // Handle read error
        *outputLength = 0;
        output[0] = '\0';
        return -1;
    }

    output[*outputLength] = '\0'; // Null-terminate the string

    // Extract the message content from the JSON
    char *start = strstr(output, "\"content\":\"");
    if (start) {
        start += strlen("\"content\":\"");
        char *end = strchr(start, '"');
        if (end) {
            *end = '\0';
            size_t contentLength = end - start;
            if (contentLength >= bufferSize) {
                // Handle content too long
                *outputLength = 0;
                output[0] = '\0';
                return -1;
            }
            memmove(output, start, contentLength + 1);  // +1 for the null-terminator
            *outputLength = contentLength;
        }
    }

    return 0;  // Success
}
