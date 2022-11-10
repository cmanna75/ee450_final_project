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
#include <signal.h>
#include<iostream>
#define SERVER_PORT 25460
using namespace std;

//socket defintion
int client_socket;

//closes sockets on cntrl^c
void interrupt_handler(int signal){
    printf("Closing Socket");
    close(client_socket);
    exit(1);
}
int main(){
    
     //handle cntrl^c
    signal(SIGINT, interrupt_handler);

    //create tcp socket to communicate with serverM, (IPv4, TCP, IP)
    if ((client_socket = socket(AF_INET,SOCK_STREAM,0)) < 0){
        printf("Error could not create socket");
        exit(1);
    }

    //server address
    struct sockaddr_in server_address;
    server_address.sin_family  = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_address.sin_port = htons(SERVER_PORT); // htons converts to network byte order(big endian)

    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);


    //connect to host
    if(connect(client_socket, (struct sockaddr*)&server_address,sizeof(server_address)) < 0){
        printf("Error connecting");
        exit(1);
    }
    getsockname(client_socket,(struct sockaddr*)&client_address,&client_length);
    printf("Port no: %u", ntohs(client_address.sin_port));

    char username[50], password[50], buffer_in[50];
    cout<<"Please enter the username: ";
    cin >> username;
    cout<<"Please enter the password: ";
    cin >>password;
    send(client_socket,username,strlen(username),0);
    recv(client_socket,buffer_in,9,0);
    printf("%s",buffer_in);
    send(client_socket,password,strlen(password),0);
    close(client_socket);
    return 0;
}
