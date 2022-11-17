#include "utilities.h"
#define UDP_PORT 21460
using namespace std;

//returns 0 if correct, 1 if username is wrong, 2 if password is wrong, 3 if both are wrong
int check_credentials(string message){
    return 0;
}
//socket defintion
int udp_socket;
void interrupt_handler(int signal){
    printf("Closing Socket");
    close(udp_socket);
    exit(1);
}
int main(){

    //handle cntrl^c
    signal(SIGINT, interrupt_handler);

    //create udp socket
    udp_socket = create_socket(SOCK_DGRAM,1,UDP_PORT);

    //receive buffer
    char buffer[102];

    //client info
    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
    socklen_t client_length = sizeof(client_address);
    int n;

    //booting up message
    printf("serverC is up and running using UDP on port %i\n", UDP_PORT);
    while(1){
        //wait for credentials
        n = recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        printf("The ServerC received an authentication request from the Main Server\n");
        //check if crednetials are valid
        
        //send response
        printf("The ServerC finished sending the response to the Main Server.\n");
    }

}
