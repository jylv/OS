#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

const char* SOCKET_PATH = "/tmp/socket";

int main() {
    int sock = 0;
    struct sockaddr_un serv_addr;
    char * hello = "Hello from client";

    if((sock = socket(AF_UNIX,SOCK_STREAM,0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path,SOCKET_PATH);

    if(connect(sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    send(sock, hello, strlen(hello),0);
    std::cout<<"Sent to server: "<< hello<< std::endl;

    close(sock);
    return 0;
}