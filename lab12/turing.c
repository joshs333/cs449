#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define PORT 50822

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
    int ret_val_bind = bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    if(bind < 0 ) {
        close(sfd);
    }
    printf("Bound and listening...\n");
    if(listen(sfd, 10) < 0) {
        printf("Error listening!\n");
        close(sfd);
        return 1;
    }
    int connfd = accept(sfd, NULL, NULL);
    if(connfd < 0) {
        close(sfd);
        printf("Error accepting\n");
        return 1;
    } else {
        printf("Accepting connections...\n");
    }
    int BUFSIZE = 2014;
    char buffer[BUFSIZE];
    strcpy(buffer, "Hello there!\n");
    send(connfd, buffer, strlen(buffer), 0);
    char* greeting[] = {
        "hello",
        "hi",
        "greetings"
    };
    char* good[] = {
        "good",
        "beautiful",
        "nice"
    };
    char* bad[] = {
        "bad",
        "dark",
        "ugly"
    };
    int ask = 0;
    int quit = 0;
    while(1) {
        int recvb = recv(connfd, buffer, 1024, 0);
        buffer[recvb - 2] = 0;
        int i = 0;
        printf("Recv: %s<\n", buffer);
        printf("%s to %s is %d", buffer, "quit", strcmp(buffer, "quit"));
        if(strcmp(buffer, "hi") == 0) {
            strcpy(buffer, "Hi, how are you?\n");
            send(connfd, buffer, strlen(buffer), 0);
            ask = 1;
            continue;
        }
        if(ask == 1) {
            while(i < 3) {
                if(strcmp(buffer, good[i]) == 0) {
                    strcpy(buffer, "That's great!\n");
                    send(connfd, buffer, strlen(buffer), 0);
                    break;
                }
                if(strcmp(buffer, bad[i]) == 0) {
                    strcpy(buffer, "That sucks!\n");
                    send(connfd, buffer, strlen(buffer), 0);
                    break;
                }
            }
            ask = 0;
        }
        if(strcmp(buffer, "quit") == 0) {
            printf("Quitting.\n");
            break;
        }
    }

    close(connfd);
    close(sfd);
    return 0;
}
