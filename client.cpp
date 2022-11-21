#include "utilities.h"
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

    //create client socket, do not bind to port
    client_socket = create_socket(SOCK_STREAM,0,0);
    //server address
    struct sockaddr_in server_address = create_address(SERVER_PORT);
    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    //connect to host
    if(connect(client_socket, (struct sockaddr*)&server_address,sizeof(server_address)) < 0){
        printf("Error connecting");
        exit(1);
    }
    getsockname(client_socket,(struct sockaddr*)&client_address,&client_length);
    //printf("Port no: %u", ntohs(client_address.sin_port));
    printf("The client is up and running.\n");
    string msg_out, username, password,msg_in;
    char buffer_in[10];
    int flag = 1;
    for(int i = 2; i >= 0; i--){
        cout<<"Please enter the username: ";
        getline(cin,username);
        cout<<"Please enter the password: ";
        getline(cin,password);
        msg_out = username + "," + password;
        send(client_socket,msg_out.c_str(),msg_out.length(),0);
        printf("%s sent an authentication request to the main server.\n", username.c_str());
        recv(client_socket,buffer_in,1,0);
        if(buffer_in[0] == FAIL_NO_USR){
            printf("%s received the result of authentication using TCP over port %u. Authentication failed: Username Does not exist\n", username.c_str(), ntohs(client_address.sin_port) );
            printf("Attempts remaining: %i", i);
        }
        else if(buffer_in[0] == FAIL_NO_PASS){
            printf("%s received the result of authentication using TCP over port %u. Authentication failed: Password does not match\n",username.c_str(), ntohs(client_address.sin_port));
            printf("Attempts remaining: %i", i);
        }
        else if(buffer_in[0] == PASS_CRED){
            printf("%s received the result of authentication using TCP over port %u. Authentication is successful\n",username.c_str(), ntohs(client_address.sin_port));
            flag = 0;
            break;
        }
    }
    //if validation passed continue if not shutdown
    if(flag){
        printf("Authentication Failed for 3 attempts. Client will shut down.\n");
    }
    else{
        printf("Please enter the course code to query: ");
    }
    close(client_socket);
    return 0;
}
