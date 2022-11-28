#include "utilities.h"
#include<iostream>
#define SERVER_PORT 25460
using namespace std;

//socket defintion
int client_socket;

//ensures valid 5 character course code
bool valid_course_code(string code){
    for( int i = 0; i < 2; i++){
        //if first two letters not capital letters return false
        if( (code[i] < 'A') || (code[i] > 'Z'))
            return false;

    }
        //if next three characters are not numbers return false
    for(int i = 2; i < 5; i++){
        if(code[i] < '0' || code[i] > '9'){
            return false;
        }
    }
    return true;
}

// 0 - error, 1 valid single course querry, 2 valid multi course querry
string check_course_querry(string code){
    //normal course querry
    if(code.length() == 5){
        if(valid_course_code(code))
            return "1";
    }
    //multi course querry
    else if(code.length() >= 11){
        int i = 0;
        int course_count = 0;
        while(i < code.length()){
            //if not valid course code
            if(!valid_course_code(code.substr(i,5))){
                return "0";
            }
            //if not space delimitted
            if( !(code[i+5] == ' ' || code[i+5] == '\0') ){
                return "0";
            }
            i = i + 6;
            course_count++;
        }
        //if less than 10 courses
        if(course_count < 10)
            return "2";
    }
    return "0";
}

//checks if category input is valid
bool check_ctg(string ctg){
    if(ctg ==  "Credit" || ctg == "Professor" || ctg == "Days" || ctg == "CourseName")
        return true;
    return false;
}

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
    //get dynamic port
    getsockname(client_socket,(struct sockaddr*)&client_address,&client_length);
    printf("The client is up and running.\n");
    string msg_out, username, password,msg_in;
    char cred_buffer_in[10]; //buffer in for credentials
    int auth_flag = 1;
    //Auth for loop gives three attempts before shutting down
    for(int i = 2; i >= 0; i--){
        cout<<"Please enter the username: ";
        getline(cin,username);
        cout<<"Please enter the password: ";
        getline(cin,password);
        msg_out = username + "," + password;
        send(client_socket,msg_out.c_str(),msg_out.length(),0);
        printf("%s sent an authentication request to the main server.\n", username.c_str());
        recv(client_socket,cred_buffer_in,1,0);
        if(cred_buffer_in[0] == FAIL_NO_USR){
            printf("%s received the result of authentication using TCP over port %u. Authentication failed: Username Does not exist\n", username.c_str(), ntohs(client_address.sin_port) );
            printf("Attempts remaining: %i\n", i);
        }
        else if(cred_buffer_in[0] == FAIL_NO_PASS){
            printf("%s received the result of authentication using TCP over port %u. Authentication failed: Password does not match\n",username.c_str(), ntohs(client_address.sin_port));
            printf("Attempts remaining: %i\n", i);
        }
        else if(cred_buffer_in[0] == PASS_CRED){
            printf("%s received the result of authentication using TCP over port %u. Authentication is successful\n",username.c_str(), ntohs(client_address.sin_port));
            auth_flag = 0;
            break;
        }
    }
    //if validation passed continue if not shutdown
    if(auth_flag){
        printf("Authentication Failed for 3 attempts. Client will shut down.\n");
    }
    else{
        //loop querry
        while(1){
            string course_code, category;
            string querry_type = "0";
            while(1){
                printf("Please enter the course code(s) to query: ");
                getline(cin,course_code);
                querry_type = check_course_querry(course_code);
                if(querry_type != "0")
                    break;
                printf("Error invalid course code format, please ensure capital letters, leave no trailing white space, less than 10 querries.\n");
            }
            msg_out = querry_type + "," + course_code;
            if(querry_type == "1"){
                while(1){
                    printf("Please enter Please enter the category (Credit / Professor / Days / CourseName): ");
                    getline(cin,category);
                    if(check_ctg(category))
                        break;
                    printf("Error invalid category, NOTE: category is case sensitive, leave no trailing white space.\n");
                }
                msg_out += "," + category;
            }
            //send to main server
            send(client_socket,msg_out.c_str(),msg_out.length(),0);
            if(querry_type == "1")
                printf("%s sent a request to the main server.\n", username.c_str());
            else
                printf("%s sent a request with multiple CourseCode to the main server.\n", username.c_str());
            char course_buffer_in[1000];
            memset(course_buffer_in,0,1000);
            //adjust input size based on querry type
            if(querry_type == "2")
                recv(client_socket,course_buffer_in,1000,0);
            else
                recv(client_socket,course_buffer_in,200,0);
            printf("The client received the response from the Main server using TCP over port %i.\n",ntohs(client_address.sin_port));
            printf("%s",course_buffer_in);
            printf("\n\n-----Start a new request-----\n");
        }   
    }
    close(client_socket);
    return 0;
}
