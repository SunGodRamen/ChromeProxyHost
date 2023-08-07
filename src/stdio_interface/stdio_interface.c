#include <stdint.h>
#include "stdio_interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

// Write data to the stdio interface
void write_stdio(const char *input, uint32_t inputLength, int stdioMsgId) {
    char buffer[BUFFER_SIZE];
    uint32_t messageLength = snprintf(buffer, sizeof(buffer), "%u:%s", stdioMsgId, input);

#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    fwrite(&messageLength, sizeof(messageLength), 1, stdout);
    fwrite(buffer, sizeof(char), messageLength, stdout);
    fflush(stdout);
}

// Read data from the stdio interface and extract message ID
uint32_t read_stdio(char *output, uint32_t bufferSize, uint32_t *outputLength) {
    uint32_t messageID = 0;  // Default ID if no ID is found
    
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
    }

    output[*outputLength] = '\0'; // Null-terminate the string
    
    // Extract the ID without using sscanf
    char *colonPosition = strchr(output, ':');
    if (colonPosition) {
        messageID = (uint32_t)strtol(output, NULL, 10);
        
        // Remove the ID and colon from the output without modifying actual content
        size_t remainingLength = *outputLength - ((colonPosition + 1) - output);
        memmove(output, colonPosition + 1, remainingLength);
        output[remainingLength] = '\0';  // Null-terminate the updated string
    }

    return messageID;  // Return the extracted message ID
}

