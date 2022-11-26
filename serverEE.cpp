#include "utilities.h"
#include <fstream>
#define UDP_PORT 23460
using namespace std;


//socket defintion
int udp_socket;

string search_course(string message){
    ifstream courses("ee.txt");
    string course_info;
    if(courses.is_open()){
        while(getline(courses,course_info)){
            //if class codes match look for specified category
            if(course_info.substr(0,5) == message.substr(0,5)){
                string ctg = message.substr(6,message.length()-6);
                int i = 0;
                if(ctg == "Credit"){
                    i = 1;
                }
                else if(ctg == "Professor"){
                    i = 2;
                }
                else if(ctg == "Days"){
                    i = 3;
                }
                else if(ctg == "CourseName"){
                    i  = 4;
                }
                int j = 5;
                int start, end;
                while(i >= 0){
                    if(course_info[j] == ','||course_info[j] == '\0'){
                        if(i == 1)
                            start = j+1;
                        else if(i == 0){
                            end = j-1;
                        }
                        i--;
                    }
                    j++;
                }
                string msg_out = "The " +ctg+" of "+message.substr(0,5)+" is "+course_info.substr(start,end-start+1)+".";
                printf("The course information has been found; %s", msg_out.c_str());
                return msg_out;
            } 
        }         
    }
    string msg_out = "Didn’t find the course: " + message.substr(0,5)+".";
    printf("%s\n", msg_out.c_str());
    return msg_out;
}
string query_courses(string message){
    string msg_out = "";
    ifstream courses("ee.txt");
    string course_info;
    int i = 0;
    while(i <= message.length()){
        int flag = 0;
        if(courses.is_open()){
           // printf("blah\n");
            while(getline(courses,course_info)){
                //if class codes match,take in call tategory information
                if(course_info.substr(0,5) == message.substr(i,5)){
                    msg_out += message.substr(i,5) + ": " + course_info.substr(6,course_info.length() - 6) + "\n";
                    printf("Found course: %s\n", message.substr(i,5).c_str());
                    flag = 1;
                    break;
                }
            }
            if(!flag)
                msg_out += "Didn’t find the course: " + message.substr(i,5) + "\n";
                printf("%s", msg_out.c_str());
        }
        i = i + 6;
        courses.clear();
        courses.seekg(0);
    }  
    printf("%s",msg_out.c_str());
    return msg_out;
}

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
    printf("serverEE is up and running using UDP on port %i\n", UDP_PORT);
    while(1){
        //wait for query from main
        n = recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        printf("The ServerEE received an authentication request from the Main Server\n");
        string message(buffer);
        string msg_out;
        //if 0 normal function - search 1 course 1 category
        if(message[0] == '0'){
            msg_out = search_course(message.substr(2,message.length()-2));
        }
        //else extra credit - search multiple courses get all categoroes
        else{
            msg_out = query_courses(message.substr(2,message.length()-2));
        }
       
        sendto(udp_socket,msg_out.c_str(),msg_out.length(),0,(struct sockaddr *) &client_address, client_length);
        //send response
        printf("The ServerC finished sending the response to the Main Server.\n");
    }
    return 0;
}