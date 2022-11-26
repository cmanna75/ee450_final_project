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
                   //printf("%c, %i\n",course_info[j], i);
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
                return msg_out;
            } 
        }         
    }
    string msg_out = "Didn’t find the course: " + message.substr(0,5);
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
                    flag = 1;
                    break;
                }
            }
            if(!flag)
                msg_out += "Didn’t find the course: " + message.substr(i,5) + "\n";
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
    /*
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
        //wait for credentials
        n = recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        string message(buffer);
        printf("The ServerEE received an authentication request from the Main Server\n");
        
        string msg_out = search_course(message);
        sendto(udp_socket,&response,1,0,(struct sockaddr *) &client_address, client_length);
        //send response
        printf("The ServerC finished sending the response to the Main Server.\n");
        */
    //search_course("EE550,CourseName");
    //}
    query_courses("EE658,EE452,EE604,EE608,EE450");
    return 0;
}