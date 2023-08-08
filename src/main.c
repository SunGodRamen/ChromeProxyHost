#include "tcp_server/tcp_server.h"
#include "stdio_interface/stdio_interface.h"
#include "logger/logger.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TCP_PORT 4000
SOCKET clientSocket;


// Monitor the stdio interface for messages from the Chrome extension
DWORD WINAPI monitor_stdio(LPVOID lpParameter) {
    char stdioMsg[BUFFER_SIZE];
    uint32_t stdioMsgLength = 0;
    uint32_t stdioMsgId = 1;

    while (1) {
        // Read the message from the stdio interface
        uint32_t currentMessageID = read_stdio(stdioMsg, BUFFER_SIZE, &stdioMsgLength);

        // If the message is not empty
        if (stdioMsgLength == 0) {
            continue;
        }

        // Log the message
        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "STDIO: Received %d bytes from client: %s\n", stdioMsgLength, stdioMsg);
        write_log(msg_buffer);

        // Send the message to the Socket.io client
        send_to_client(clientSocket, stdioMsg, stdioMsgLength);

    }

    return 0;
}

// Setup and Monitor the TCP server for messages from the AutoHotkey script
DWORD WINAPI monitor_tcp(LPVOID lpParameter) {
    uint32_t stdioMsgId = 1;
    // Initialize TCP server
    SOCKET serverSocket = init_server(TCP_PORT);
    clientSocket = accept_connection(serverSocket);

    // Set the socket to non-blocking mode with a timeout for receive
    set_socket_nonblocking_with_timeout(clientSocket, SOCKET_TIMEOUT_MS);

    char clientMsg[BUFFER_SIZE];
    while (1) {
        // Receive message from the Socket.io client
        int bytesRead = receive_from_client(clientSocket, clientMsg, sizeof(clientMsg));
        // If the message is not empty
        if (bytesRead > 0) {
            // Null terminate the received message
            clientMsg[bytesRead] = '\0';

            // Log the message
            char msg_buffer[512];
            snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Received %d bytes from client: %s\n", bytesRead, clientMsg);
            write_log(msg_buffer);

            // Send the message to the stdio interface
            write_stdio(clientMsg, bytesRead, stdioMsgId++);
        }
    }
    cleanup_server(serverSocket, clientSocket);
    return 0;
}

int main() {

    init_logger();

    // Create threads to monitor the stdio interface and TCP server
    HANDLE stdioThread = CreateThread(NULL, 0, monitor_stdio, NULL, 0, NULL);
    HANDLE tcpThread = CreateThread(NULL, 0, monitor_tcp, NULL, 0, NULL);

    // Wait for threads to exit
    WaitForSingleObject(stdioThread, INFINITE);
    WaitForSingleObject(tcpThread, INFINITE);
    CloseHandle(stdioThread);
    CloseHandle(tcpThread);

    close_logger();

    return 0;
}
