#include <arpa/inet.h>   // sockaddr_in, inet_addr().
#include <sys/socket.h>  // socket(), connect(), send(), recv().
#include <unistd.h>      // close().

#include <cstdio>   // printf(), fprintf().
#include <cstdlib>  // atoi(), exit().
#include <cstring>  // memset().

#include "../die_with_error.hpp"  // DieWithError(). Error handling function.

const int RCVBUFSIZE = 32;  // Size of receive buffer.

int main(int argc, char *argv[]) {
    int sock;                             // Socket descriptor.
    struct sockaddr_in echoServAddr;      // Echo server address.
    uint16_t echoServPort = 5000;         // Echo server port.
    char *servIp = "127.0.0.1";           // Server IP address (dotted quad).
    char *echoString = "Hello world!\0";  // String to send to echo server.
    int tmp;

    // if(argc < 3 or 4 < argc) {
    //     fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
    //     exit(1);
    // }
    // servIp = argv[1];
    // echoString = argv[2];
    // echoServPort = (argc == 4 ? atoi(argv[3]) : 7);  // Echo server port. 7 is the well-known port for the echo service.

    // Create a reliable, stream socket using TCP.
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // Socket descriptor.
    if(sock < 0) DieWithError("socket() failed");

    // Construct the server address structure.
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIp);
    echoServAddr.sin_port = htons(echoServPort);

    // Establish the connection to the echo server.
    tmp = connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
    if(tmp < 0) DieWithError("connect() failed");

    // Send the string to the server.
    unsigned int echoStringLen = strlen(echoString);  // Length of string to echo.
    tmp = send(sock, echoString, echoStringLen, 0);
    if(tmp != echoStringLen) DieWithError("send() sent a different number of bytes than expected");

    // Receive the same string back from the server.
    int totalBytesRcvd = 0;  // Total bytes read.
    printf("Received: ");
    while(totalBytesRcvd < echoStringLen) {
        char echoBuffer[RCVBUFSIZE];                                // Buffer for echo string.
        int bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0);  // Bytes read in single recv().
        if(bytesRcvd <= 0) DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;
        echoBuffer[bytesRcvd] = '\0';
        printf(echoBuffer);
    }
    printf("\n");

    // Stop connection.
    close(sock);
    exit(0);
}
