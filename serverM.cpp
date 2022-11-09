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
#define UDP_PORT 24460
#define TCP_PORT 25460
using namespace std;
int main(){

    //create tcp socket to communicate with clients, (IPv4, TCP, IP)
    int tcp_socket = socket(AF_INET,SOCK_STREAM,0);

    //address data structure for bind
    struct sockaddr_in tcp_address;
    tcp_address.sin_family  = AF_INET;
    tcp_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    tcp_address.sin_port = htons(TCP_PORT); // htons converts to network byte order(big endian)

    //bind port no and address to socket
    bind(tcp_socket,(struct sockaddr *)&tcp_address, sizeof(tcp_address));

    //listen for potential clients, given queue limit of 3
    listen(tcp_socket,3);
    
    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    //create child socket once client request is found
    int child_socket = accept(tcp_socket,(struct sockaddr *)&client_address, &client_length);

    printf("Connected!");

    close(child_socket);

    printf("Socket closed");
    return 0;
}
