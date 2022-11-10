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

    //address data structure for bind
    struct sockaddr_in server_address;
    server_address.sin_family  = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_address.sin_port = htons(SERVER_PORT); // htons converts to network byte order(big endian)

    //connect to host
    connect(client_socket, (struct sockaddr*)&server_address,sizeof(server_address));

    close(client_socket);
    return 0;
}
