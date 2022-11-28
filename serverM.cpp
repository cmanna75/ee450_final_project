#include "utilities.h"
#define UDP_PORT 24460
#define SERVC_PORT 21460
#define SERVCS_PORT 22460
#define SERVEE_PORT 23460
#define TCP_PORT 25460
using namespace std;

//socket defintions
int tcp_socket,child_socket,udp_socket;


string encrypt_msg(string msg){
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

    //create udp_client
    udp_socket = create_socket(SOCK_DGRAM,1,UDP_PORT);

    //serverC address
    struct sockaddr_in servC_address = create_address(SERVC_PORT);
    socklen_t servC_length = sizeof(servC_address);

    //serverCS address
    struct sockaddr_in servCS_address = create_address(SERVCS_PORT);
    socklen_t servCS_length = sizeof(servCS_address);

    //serverEE address
    struct sockaddr_in servEE_address = create_address(SERVEE_PORT);
    socklen_t servEE_length = sizeof(servEE_address);

    //listen for potential clients, given queue limit of 3
    if(listen(tcp_socket,3)<0){
        printf("Error listening");
        exit(1);
    }

    //client address
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    printf("The main server is up and running.\n");
    //main loop
    while(1){
        //create child socket once client request is found
        if( (child_socket = accept(tcp_socket,(struct sockaddr *)&client_address, &client_length)) < 0){
            printf("Error accepting");
            exit(1);
        }
        int flag = 1;
        char buffer_in[102];
        for(int i = 2; i >= 0; i--){
            string username;
            memset(buffer_in,0,102);
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
            //printf("%s\n",enc_auth.c_str());
            sendto(udp_socket, enc_auth.c_str(),enc_auth.length(),0,(struct sockaddr *) &servC_address, servC_length);
            printf("The main server sent an authentication request to serverC\n");
            recvfrom(udp_socket,buffer_in,1,0,(struct sockaddr *) &servC_address, &servC_length);
            printf("The main server received the result of the authentication request from ServerC using UDP over port %u\n",ntohs(servC_address.sin_port));
            send(child_socket,buffer_in,1,0);
            printf("The main server sent the authentication result to the client.\n");
            if(buffer_in[0] == PASS_CRED){
                flag = 0;
                break;
            }
        }
        if(!flag){
            while(1){
                printf("Course time!\n");
                memset(buffer_in,0,102);
                recv(child_socket,buffer_in,70,0);
                string course_querry(buffer_in);
                string course_response;
                char course_buffer_in[1000];
                printf("char: %s\n", buffer_in);
                printf("%s\n", course_querry.c_str());
                memset(buffer_in,0,102);
                memset(course_buffer_in,0,1000);

                //normal querry 1 course, 1 category
                if(course_querry[0] == '1'){
                    if(course_querry.substr(2,2) == "EE"){
                        printf("EE yay!\n");
                        sendto(udp_socket,course_querry.c_str(),course_querry.length(),0,(struct sockaddr *) &servEE_address, servEE_length);
                        recvfrom(udp_socket,course_buffer_in,200,0,(struct sockaddr *) &servEE_address, &servEE_length);
                        course_response = string(course_buffer_in);
                        memset(course_buffer_in,0,1000);
                        printf("%s\n",course_response.c_str());
                    }
                    else if(course_querry.substr(2,2) == "CS"){
                        printf("CS yay!\n");
                        sendto(udp_socket,course_querry.c_str(),course_querry.length(),0,(struct sockaddr *) &servCS_address, servCS_length);
                        recvfrom(udp_socket,course_buffer_in,200,0,(struct sockaddr *) &servCS_address, &servCS_length);
                        course_response = string(course_buffer_in);
                        memset(course_buffer_in,0,1000);
                    }
                    //cannot find department 
                    else{
                        course_response = "Didn’t find the course: " + course_querry.substr(2,5) + "\n";
                    }
                }
                //EC multiple courses
                else if(course_querry[0] == '2'){

                    //create all class array, create EE string, create CS string
                    string courses[9];
                    string EE_querry("2");
                    string CS_querry("2");
                    int course_count = 0;
                    for(int i = 2; i < course_querry.length(); i = i + 6){
                        courses[course_count] = course_querry.substr(i,5);
                        course_count++;
                        if(course_querry.substr(i,2) == "EE"){
                            EE_querry += "," + course_querry.substr(i,5);
                        }
                        else if(course_querry.substr(i,2) == "CS"){
                            CS_querry += "," + course_querry.substr(i,5);
                        }
                    }
                    if(EE_querry.length() > 1){
                        //send EE
                        sendto(udp_socket, EE_querry.c_str(), EE_querry.length(),0,(struct sockaddr *) &servEE_address, servEE_length); 

                        //receive EE
                        recvfrom(udp_socket,course_buffer_in,1000,0,(struct sockaddr *) &servEE_address, &servEE_length);
                        EE_querry = string(course_buffer_in);
                        memset(course_buffer_in,0,1000);
                    }
                    if(CS_querry.length() > 1){
                        //send CS
                        sendto(udp_socket, CS_querry.c_str(), CS_querry.length(),0,(struct sockaddr *) &servCS_address, servCS_length); 
                        //receive CS
                        recvfrom(udp_socket,course_buffer_in,1000,0,(struct sockaddr *) &servCS_address, &servCS_length);
                        CS_querry = string(course_buffer_in);
                        memset(course_buffer_in,0,1000);
                    }
                    //printf("%s blah\n",EE_querry.c_str());
                    //create response string for client
                    course_response = "CourseCode: Credits, Professor, Days, Course Name\n"; //header line
                    for(int i = 0; i < course_count; i++){
                        if(courses[i].substr(0,2) == "EE"){
                            int j = 0;
                            while(EE_querry[j] != '\n'){
                                j++;
                            }
                            course_response += EE_querry.substr(0,j+1);
                            if(j != (EE_querry.length()-1))
                                EE_querry = EE_querry.substr(j+1,EE_querry.length()-j-1);
                        }
                        else if(courses[i].substr(0,2) == "CS"){ 
                            int j = 0;
                            while(CS_querry[j] != '\n'){
                                j++;
                            }
                            course_response += CS_querry.substr(0,j+1);
                            if(j != (CS_querry.length()-1))
                                CS_querry = CS_querry.substr(j+1,CS_querry.length()-j-1);
                        }
                        else{
                            course_response +=  "Didn’t find the course: " + courses[i].substr(2,5) + "\n";
                        }
                    }

                }
                //else error occured
                else{
                    course_response = "Error invalid Querry Type\n";
                }
                //send querry response to client
                send(child_socket,course_response.c_str(),course_response.length(),0);
                printf("The main server sent the query information to the client.\n");
            }
        } 
        close(child_socket);

        printf("Socket closed");
    }
    return 0;
}
