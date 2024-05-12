#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>

const char* SOCKET_PATH = "/tmp/socket";

int main() {
    //store file descriptors for the server socket and the client socket
    int server_fd,new_socket;
    //store the address information for the Unix domain socket.
    struct sockaddr_un address;
    //used later for setting socket options
    int opt = 1;
    int addrlen = sizeof(address);
    //store data received from the client.
    char buffer[1024] = {0};
    //store the number of bytes received from the client
    ssize_t bytes;

    /*
     * Creates a Unix domain socket using the socket() system call
     * If the socket creation fails, it prints an error message and exits the program.
     */
    if((server_fd = socket(AF_UNIX,SOCK_STREAM,0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    /*
     * Sets socket options using the setsockopt() system call.
     * This line enables reusing the socket address and sets the socket to non-blocking mode.
     */
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | O_NONBLOCK, &opt,sizeof(opt))) {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    }

    //Sets the address family of the address structure to AF_UNIX, indicating that it's a Unix domain socket.
    address.sun_family = AF_UNIX;
    //Copies the socket path (SOCKET_PATH) to the sun_path member of the address structure.
    strncpy(address.sun_path,SOCKET_PATH,sizeof(address.sun_path)-1);

    /*
     * Binds the socket to the specified address (SOCKET_PATH).
     * If the binding fails, it prints an error message and exits the program.
     */
    if(bind(server_fd,(struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /*
     * Listens for incoming connections on the server socket.
     * It sets the maximum number of pending connections to 3.
     * If the listening fails, it prints an error message and exits the program.
     */
    if(listen(server_fd,3)<0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout<<"Server listening on "<<SOCKET_PATH<<std::endl;

    /*
     * Accepts an incoming connection on the server socket.
     * If the acceptance fails, it prints an error message and exits the program.
     */
    if((new_socket = accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    /*
     * Receives data from the client socket and stores it in the buffer.
     * The number of bytes received is stored in bytes.
     */
    bytes = recv(new_socket,buffer,1024,0);
    if(bytes<0) {
        perror("recv");
    }else {
        buffer[bytes] = '\0';
        std::cout<<"Received from client: "<<buffer<<std::endl;
    }

    //Closes the client socket.
    close(new_socket);
    //Closes the server socket.
    close(server_fd);

    //Removes the socket file from the filesystem.
    unlink(SOCKET_PATH);

    return 0;
}
