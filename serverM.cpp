#include "utilities.h"
#define UDP_PORT 24460
#define SERVC_PORT 21460
#define TCP_PORT 25460
using namespace std;

//socket defintions
int tcp_socket,child_socket,udp_socket;

//closes sockets on cntrl^c
void interrupt_handler(int signal){
    printf("Closing Socket");
    close(child_socket);
    close(tcp_socket);
    exit(1);
}

int main(){
    //int tcp_socket, child_socket;

    //handle cntrl^c
    signal(SIGINT, interrupt_handler);
    tcp_socket = create_socket(SOCK_STREAM,1,TCP_PORT);
    /*
    //create tcp socket to communicate with clients, (IPv4, TCP, IP)
    if((tcp_socket = socket(AF_INET,SOCK_STREAM,0))< 0){
        printf("Error could not create socket");
        exit(1);
    }

    //address data structure for bind
    struct sockaddr_in tcp_address;
    tcp_address.sin_family  = AF_INET;
    tcp_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    tcp_address.sin_port = htons(TCP_PORT); // htons converts to network byte order(big endian)

    //bind port no and address to socket
    if(bind(tcp_socket,(struct sockaddr *)&tcp_address, sizeof(tcp_address))<0){
        printf("Error could not bind");
        exit(1);
    }
    */
    udp_socket = create_socket(SOCK_DGRAM,1,UDP_PORT);

    struct sockaddr_in servC_address = create_address(SERVC_PORT);
    socklen_t servC_length = sizeof(servC_address);
    
    //create udp socket
    /*
    if((udp_socket = socket(AF_INET,SOCK_DGRAM,0))< 0){
        printf("Error could not create socket");
        exit(1);
    }

     //address data structure for bind
    struct sockaddr_in udp_address;
    udp_address.sin_family  = AF_INET;
    udp_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    udp_address.sin_port = htons(UDP_PORT); // htons converts to network byte order(big endian)

    //address data structure for bind
    struct sockaddr_in servC_address;
    servC_address.sin_family  = AF_INET;
    servC_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servC_address.sin_port = htons(SERVC_PORT); // htons converts to network byte order(big endian)
    socklen_t servC_length = sizeof(servC_address);

    /*
     //bind port no and address to socket
    if(bind(udp_socket,(struct sockaddr *)&udp_address, sizeof(udp_address))<0){
        printf("Error could not bind");
        exit(1);
    }
    */

    //listen for potential clients, given queue limit of 3
    if(listen(tcp_socket,3)<0){
        printf("Error listening");
        exit(1);
    }
    
    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    //create child socket once client request is found
    if( (child_socket = accept(tcp_socket,(struct sockaddr *)&client_address, &client_length)) < 0){
        printf("Error accepting");
        exit(1);
    }
    char username[50], password[50], buffer_in[100];
    printf("Connected!");
    //recv(child_socket,buffer_in,100,0):
    int n;
    n = recv(child_socket,username,50,0);
    if(n){
        send(child_socket,"received",9,0);
    }
    else{
        send(child_socket,"error",5,0);
    }
    //recv(child_socket,password,50,0);
    printf("username: %s password: %s", username,password);
    
    //char buffer_in[50];
    sendto(udp_socket, "test",4,0,(struct sockaddr *) &servC_address, servC_length);
    recvfrom(udp_socket,buffer_in,102,0,(struct sockaddr *) &servC_address, &servC_length);
    printf("%s",buffer_in);

    //close(child_socket);

    printf("Socket closed");
    return 0;
}
