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
#include<signal.h>
#define UDP_PORT 21460
using namespace std;

//socket defintion
int udp_socket;

int main(){

    if((udp_socket = socket(AF_INET,SOCK_DGRAM,0))< 0){
        printf("Error could not create socket");
        exit(1);
    }

     //address data structure for bind
    struct sockaddr_in udp_address;
    udp_address.sin_family  = AF_INET;
    udp_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    udp_address.sin_port = htons(UDP_PORT); // htons converts to network byte order(big endian)

     //bind port no and address to socket
    if(bind(udp_socket,(struct sockaddr *)&udp_address, sizeof(udp_address))<0){
        printf("Error could not bind");
        exit(1);
    }

    //receive buffer
    char buffer[102];

    //client info
    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
    socklen_t client_length = sizeof(client_address);
    int n;
    while(1){
        //wait for credentials
        n = recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        printf("blah\n");
        printf("%s\n", buffer);
        //check if crednetials are valid

        //send response

    }



}
