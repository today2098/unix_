#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>

int main() {
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int tmp;

    // ソケットの作成.
    int sock0 = socket(PF_INET, SOCK_STREAM, 0);
    if(sock0 < 0) {
        perror("socket");
        return 1;
    }

    // ソケットの設定.
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    tmp = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if(tmp < 0) {
        perror("bind");
        return 1;
    }

    listen(sock0, 5);

    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);

    write(sock, "HELLO", 5);

    close(sock);

    close(sock0);
    return 0;
}