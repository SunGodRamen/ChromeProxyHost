#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <winsock2.h>

#define SOCKET_TIMEOUT_MS 10

// Initialize the server and start listening
SOCKET init_server(int port);

// Accept a connection
SOCKET accept_connection(SOCKET serverSocket);

// Set the socket to non-blocking mode with a timeout for receive
void set_socket_nonblocking_with_timeout(SOCKET socket, int timeout_ms);

// Receive data from a client
int receive_from_client(SOCKET clientSocket, char *buffer, int bufferSize);

// Send a response to a client
void send_to_client(SOCKET clientSocket, const char *response, int responseLength);

// Cleanup and close the server
void cleanup_server(SOCKET serverSocket, SOCKET clientSocket);

#endif
