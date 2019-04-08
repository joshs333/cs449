#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define PORT 50820

int main(int argc, char** argv) {
    int sfd;
    struct sockaddr_in addr;
    sfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        printf("Error: Unable to make socket!\n");
        return 1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = INADDR_ANY; //automatically find IP
    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    printf("Bound and listening...\n");
    if(listen(sfd, 10) < 0) {
        printf("Error listening!\n");
        return 1;
    }
    int connfd = accept(sfd, NULL, NULL);
    if(connfd < 0) {
        printf("Error accepting\n");
        return 1;
    } else {
        printf("Accepting connections...\n");
    }
    char buffer[1024];
    strcpy(buffer, "Hello there!\n");
    send(connfd, buffer, strlen(buffer), 0);

    return 0;
}
