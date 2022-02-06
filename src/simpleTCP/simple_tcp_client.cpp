#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

int main() {
    struct sockaddr_in server;
    int sock;
    char buf[32];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    memset(buf, 0, sizeof(buf));
    n = read(sock, buf, sizeof(buf));

    printf("%d, %s\n", n, buf);

    close(sock);
    return 0;
}
