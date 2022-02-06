#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    pid_t mypid;
    char cmd[1000];

    mypid = getpid();
    printf("pid = %d\n", mypid);
    if(snprintf(cmd, sizeof(cmd), "ps -l -p %d", mypid) >= sizeof(cmd)) {
        fprintf(stderr, "Too long command line (pid = %d).\n", mypid);
        exit(1);
    }

    system(cmd);
}
