#include "tcp_server.h"
#include "../logger/logger.h"
#include <stdio.h>

SOCKET init_server(int port) {
    WSADATA wsaData;

    // Initialize WinSock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {

        char buffer[512];
        snprintf(buffer, sizeof(buffer), "TCP: Failed to initialize WinSock. Error Code: %d", WSAGetLastError());
        write_log(buffer);

        exit(1);  // Exit the program if WinSock initialization fails
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {

        char buffer[512];
        snprintf(buffer, sizeof(buffer), "TCP: Failed to create socket. Error Code: %d\n", WSAGetLastError());
        write_log(buffer);

        exit(1);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        
        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Bind failed. Error Code: %d\n", WSAGetLastError());
        write_log(msg_buffer);

        closesocket(serverSocket);
        exit(1);
    }

    // Listen for connections
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        
        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Listen failed. Error Code: %d\n", WSAGetLastError());
        write_log(msg_buffer);
        
        closesocket(serverSocket);
        exit(1);
    }

    return serverSocket;
}

SOCKET accept_connection(SOCKET serverSocket) {
    struct sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        
        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Accept failed. Error Code: %d\n", WSAGetLastError());
        write_log(msg_buffer);
        
        closesocket(serverSocket);
        exit(1);
    }
    
    return clientSocket;
}

void set_socket_nonblocking_with_timeout(SOCKET socket, int timeout_ms) {
    // Set socket to non-blocking mode
    u_long mode = 1;  // 1 for non-blocking, 0 for blocking
    if (ioctlsocket(socket, FIONBIO, &mode) != NO_ERROR) {

        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Failed to set socket to non-blocking mode. Error Code: %d\n", WSAGetLastError());
        write_log(msg_buffer);

        closesocket(socket);
        exit(1);
    }

    // Set socket receive timeout
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms)) == SOCKET_ERROR) {

        char msg_buffer[512];
        snprintf(msg_buffer, sizeof(msg_buffer), "TCP: Failed to set socket receive timeout. Error Code: %d\n", WSAGetLastError());
        write_log(msg_buffer);

        closesocket(socket);
        exit(1);
    }
}

int receive_from_client(SOCKET clientSocket, char *buffer, int bufferSize) {
    return recv(clientSocket, buffer, bufferSize, 0);
}

void send_to_client(SOCKET clientSocket, const char *response, int responseLength) {
    send(clientSocket, response, responseLength, 0);
}

void cleanup_server(SOCKET serverSocket, SOCKET clientSocket) {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
