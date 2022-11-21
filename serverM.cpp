#include "utilities.h"
#define UDP_PORT 24460
#define SERVC_PORT 21460
#define TCP_PORT 25460
using namespace std;

//socket defintions
int tcp_socket,child_socket,udp_socket;


string encrypt_msg(string msg){
    //printf("%s\n",msg.c_str());
    printf("18: %c\n",msg[18]);
    string enc_msg;
    int n = msg.length();
    int key = 0;
    for(int i = 0; i < n; i++){
        char ch = msg[i];
        //printf("%c %i %lu f18: %c\n",msg[i],i,msg.length(),msg[18]);
        //captial
        if( (ch >= 'A') && (ch <= 'Z')){
            key = msg[i] + 4;
            if(key > 'Z'){
                key = key - 'Z' + 'A'- 1;
            }
            enc_msg += key;
        }
        //lowercase
        else if( (ch >= 'a') && (ch <= 'z')){
            key = msg[i] + 4;
            if(key > 'z'){
                key = key - 'z' + 'a'- 1;
            }
            enc_msg += key;
        }
        //number
        else if( (ch >= '0') && (ch <= '9')){
            key = ch + 4;
            if(key > '9'){
                key = key - '9' + '0' - 1;
            }
            enc_msg  += key;
        }
        //special character make the same
        
        else{
            enc_msg += ch;
        }
    }
    return enc_msg;
}

//closes sockets on cntrl^c
void interrupt_handler(int signal){
    printf("Closing Socket");
    close(child_socket);
    close(tcp_socket);
    close(udp_socket);
    exit(1);
}

int main(){
    //int tcp_socket, child_socket;

    //handle cntrl^c
    signal(SIGINT, interrupt_handler);

    //create tcp_server
    tcp_socket = create_socket(SOCK_STREAM,1,TCP_PORT);
    printf("tcp good\n");
    //create udp_client
    udp_socket = create_socket(SOCK_DGRAM,1,UDP_PORT);
    printf("udp good\n");
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
    printf("The main server is up and running.\n");
    //create child socket once client request is found
    if( (child_socket = accept(tcp_socket,(struct sockaddr *)&client_address, &client_length)) < 0){
        printf("Error accepting");
        exit(1);
    }
    char buffer_in[102];
    string username;
    int n = recv(child_socket,buffer_in,102,0);
    string auth(buffer_in);
    memset(buffer_in,0,102);
    for(int i = 0; i < n; i++){
        if(auth[i] == ','){
            username = auth.substr(0,i);
            break;
        }
    }
    printf("The main server received the authentication for %s using TCP over port %u\n",username.c_str(), ntohs(client_address.sin_port));
    string enc_auth = encrypt_msg(auth);
    printf("%s\n",enc_auth.c_str());
    sendto(udp_socket, enc_auth.c_str(),enc_auth.length(),0,(struct sockaddr *) &servC_address, servC_length);
    recvfrom(udp_socket,buffer_in,1,0,(struct sockaddr *) &servC_address, &servC_length);
    send(child_socket,buffer_in,1,0);

    close(child_socket);

    printf("Socket closed");
    
    return 0;
}
