#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define SERVER_PORT 25460
using namespace std;
int main(){
    
    //create tcp socket to communicate with serverM, (IPv4, TCP, IP)
    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    //server address
    struct sockaddr_in server_address;
    server_address.sin_family  = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_address.sin_port = htons(SERVER_PORT); // htons converts to network byte order(big endian)

    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);


    //connect to host
    connect(client_socket, (struct sockaddr*)&server_address,sizeof(server_address));
    getsockname(client_socket,(struct sockaddr*)&client_address,&client_length);
    printf("Port no: %u", ntohs(client_address.sin_port));
    close(client_socket);
    return 0;
}
