#include "utilities.h"
#define UDP_PORT 24460
#define SERVC_PORT 21460
#define TCP_PORT 25460
using namespace std;

//socket defintions
int tcp_socket,child_socket,udp_socket;

string encrypt(string msg){
    string enc_msg;
    int key = 0;
    for(int i = 0; i < message.length(); i++){
        //captial
        if( (msg[i] >= 'A') && (msg[i] <= "Z")){
            key = msg[i] + 4;
            if(key > "Z"){
                key = key - "Z" + "A"- 1;
            }
            enc_msg[i] = char(key);
        }
        //lowercase
        else if( (msg[i] >= 'a') && (msg[i] <= "z")){
            key = msg[i] + 4;
            if(key > "z"){
                key = key - "z" + "a"- 1;
            }
            enc_msg[i] = char(key);
        }
        //number
        else if( (msg[i] >= '0') && (msg[i] <= "9")){
            key = msg[i] + 4;
            if(key > "9"){
                key = key - "9" + "0" - 1;
            }
            enc_msg[i] = char(key);
        }
        //special character make the same
        else{
            enc_msg[i] = msg[i];
        }
    }
    return enc_msg;
}

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

    //create tcp_server
    tcp_socket = create_socket(SOCK_STREAM,1,TCP_PORT);

    //create udp_client
    udp_socket = create_socket(SOCK_DGRAM,1,UDP_PORT);

    //serverC address
    struct sockaddr_in servC_address = create_address(SERVC_PORT);
    socklen_t servC_length = sizeof(servC_address);

    //listen for potential clients, given queue limit of 3
    if(listen(tcp_socket,3)<0){
        printf("Error listening");
        exit(1);
    }

    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    printf("The main server is up and running.\n")
    //create child socket once client request is found
    if( (child_socket = accept(tcp_socket,(struct sockaddr *)&client_address, &client_length)) < 0){
        printf("Error accepting");
        exit(1);
    }
    string auth,username;
    n = recv(child_socket,auth.c_str(),102,0);
    for(int i = 0; i < n; i++){
        if(auth[i] == ","){
            username = auth.substr(0,i);
            break;
        }
    }
    printf("The main server received the authentication for %s using TCP over port %u\n",username, ntohs(client_address.sin_port))

    //encrypt
    string enc_auth = encrypt(auth);
    printf("%s\n",enc_auth);
    
    //char buffer_in[50];
    sendto(udp_socket, "test",4,0,(struct sockaddr *) &servC_address, servC_length);
    recvfrom(udp_socket,buffer_in,102,0,(struct sockaddr *) &servC_address, &servC_length);
    printf("%s",buffer_in);

    close(child_socket);

    printf("Socket closed");
    return 0;
}
