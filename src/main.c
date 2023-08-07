#include "tcp_server/tcp_server.h"
#include "stdio_interface/stdio_interface.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

SOCKET clientSocket;

uint32_t stdioMsgId = 1; // Track last received ID

// Monitor the stdio interface for messages from the Chrome extension
DWORD WINAPI monitor_stdio(LPVOID lpParameter) {
    char stdioMsg[BUFFER_SIZE];
    uint32_t stdioMsgLength = 0;

    while (1) {
        uint32_t currentMessageID = read_stdio(stdioMsg, BUFFER_SIZE, &stdioMsgLength);
        if (stdioMsgLength > 0 && currentMessageID != stdioMsgId) {
            stdioMsgId = currentMessageID;  // Update the last received ID
            send_to_client(clientSocket, stdioMsg, stdioMsgLength);
        }
    }

    return 0;
}

// Setup and Monitor the TCP server for messages from the AutoHotkey script
DWORD WINAPI monitor_tcp(LPVOID lpParameter) {
    // Initialize TCP server and start listening
    SOCKET serverSocket = init_server(PORT);
    clientSocket = accept_connection(serverSocket);

    // Set the socket to non-blocking mode with a timeout for receive
    set_socket_nonblocking_with_timeout(clientSocket, SOCKET_TIMEOUT_MS);

    char clientMsg[BUFFER_SIZE];

    while (1) {
        int bytesRead = receive_from_client(clientSocket, clientMsg, sizeof(clientMsg));
        if (bytesRead > 0) {
            clientMsg[bytesRead] = '\0';  // Null terminate the received message
            write_stdio(clientMsg, bytesRead, stdioMsgId++);
        }

    }

    cleanup_server(serverSocket, clientSocket);
    return 0;
}

int main() {
    HANDLE stdioThread = CreateThread(NULL, 0, monitor_stdio, NULL, 0, NULL);
    HANDLE tcpThread = CreateThread(NULL, 0, monitor_tcp, NULL, 0, NULL);

    WaitForSingleObject(stdioThread, INFINITE);
    WaitForSingleObject(tcpThread, INFINITE);

    CloseHandle(stdioThread);
    CloseHandle(tcpThread);
    
    return 0;
}
