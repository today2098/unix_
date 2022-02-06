#include <arpa/inet.h>   // sockaddr_in, inet_ntoa().
#include <sys/socket.h>  // socket(), bind(), connect(), recv(), send().
#include <unistd.h>      // close().

#include <cstdio>   // printf(), fprintf().
#include <cstdlib>  // atoi(), exit().
#include <cstring>  // memset().

#include "../die_with_error.hpp"

const int RCVBUFSIZE = 32;  // Size of receive buffer.
const int MAXPENDING = 5;   // Maximum outstanding connection requests.

// TCP client handling function.
void HandleTCPClient(int clntSocket) {
    char echoBuffer[RCVBUFSIZE];  // Buffer for echo string.
    int recvMsgSize;              // Size of received message.
    int tmp;

    // Receive message from client.
    recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
    if(recvMsgSize < 0) DieWithError("recv() failed");

    // Send received string and receive again until end of transmission.
    while(recvMsgSize > 0) {  // zero indicates end of transmission.
        // Echo message back to client.
        tmp = send(clntSocket, echoBuffer, recvMsgSize, 0);
        if(tmp != recvMsgSize) DieWithError("send() failed");

        // See if there is more data to receive/
        recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
        if(recvMsgSize < 0) DieWithError("recv() failed");
    }

    // Close client socket.
    close(clntSocket);
}

int main(int argc, char *argv[]) {
    int servSock;                     // Socket descriptor for server.
    struct sockaddr_in echoServAddr;  // Local address.
    uint16_t echoServPort = 5000;     // Server port.
    int tmp;

    // if(argc != 2) {
    //     fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
    //     exit(1);
    // }
    // echoServPort=atoi(argv[1]);

    // Create socket for incoming connections.
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(servSock < 0) DieWithError("socket() failed");

    // Construct local address structure.
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;                 // Internet address family.
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Any incoming interface.
    echoServAddr.sin_port = htons(echoServPort);       // Local port.

    // Bind to the local address.
    tmp = bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
    if(tmp < 0) DieWithError("bind() failed");

    // Mark the socket so it will listen for incoming connections.
    tmp = listen(servSock, MAXPENDING);
    if(tmp < 0) DieWithError("listen() failed");

    while(1) {
        int clntSock;                                 // Socket descriptor for client.
        struct sockaddr_in echoClntAddr;              // Client address.
        unsigned int clntLen = sizeof(echoClntAddr);  // Length of client address data structure.

        // Wait for a client to connect.
        clntSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen);
        if(clntSock < 0) DieWithError("accept() failed");

        // clntSock is connected to a client!
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock);
    }

    /* NOT REACHED */
}
