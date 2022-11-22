#include "utilities.h"
#include <fstream>
#define UDP_PORT 21460
using namespace std;

//returns 0 if correct, 1 if username is wrong, 2 if password is wrong, 3 if both are wrong
char check_credentials(string message){
    ifstream creds("cred.txt");
    string enc;
    char flag = FAIL_NO_USR;
    //printf("%s\n", message.c_str());
    if(creds.is_open()){
        //compare each string
        while(getline(creds,enc)){
            //remove delimtter
            //enc.erase(enc.size()-1,1);
            //printf("%s\n",enc.c_str());
            //compare, //if username is correct set flag to 1
            int i = 0;
            while( (i < message.length()) && (i < enc.length())){
                //if mismatch do not check rest of string
                if(message[i] != enc[i]){
                    if(flag == FAIL_NO_PASS) //if usernames are macth but password is wrong do no iterate through remaining credentials will not match
                        return flag;
                    break;
                }
                //username is correct
                else if(message[i] == ',')
                    flag = FAIL_NO_PASS;
                else if( (i == message.length()-1) && i == enc.length()-1)
                    return PASS_CRED;
                i++;
            }
        }
    }
    return flag;
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
        memset(buffer,0,102);
        n = recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        string message(buffer);
        //printf("message: %s\n",message.c_str());
        printf("The ServerC received an authentication request from the Main Server\n");

        char response = check_credentials(message);
        //check if crednetials are valid, and send to main server
        sendto(udp_socket,&response,1,0,(struct sockaddr *) &client_address, client_length);
        //send response
        printf("The ServerC finished sending the response to the Main Server.\n");
    }
}
