#include "utilities.h"
#include <fstream>
#define UDP_PORT 23460
using namespace std;


//socket defintion
int udp_socket;

//searches for singlular course, with category
string search_course(string message){

    //open file
    ifstream courses("ee.txt");
    string course_info;
    if(courses.is_open()){
        //go through file
        while(getline(courses,course_info)){
            //remove carraige return /r
            if(course_info[course_info.length()-1] == '\r'){
                course_info.erase(course_info.length()-1,1);
            }
            //if class codes match look for specified category
            if(course_info.substr(0,5) == message.substr(0,5)){
                //ctg sub string
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
                int start = 0;
                int end = 0;
                //loop to find start and end of substring
                while(i >= 0){
                    if(course_info[j] == ','|| j == course_info.length()){
                        if(i == 1)
                            start = j+1;
                        else if(i == 0){
                            end = j-1;
                        }
                        i--;
                    }
                    j++;
                }
                string msg_out = "The " +ctg+" of "+message.substr(0,5)+" is "+course_info.substr(start,end-start+1)+".\n";
                printf("The course information has been found; %s", msg_out.c_str());
                return msg_out;
            } 
        }         
    }
    //if no course found, default message
    string msg_out = "Didn’t find the course: " + message.substr(0,5)+".";
    printf("%s\n", msg_out.c_str());
    return msg_out;
}

//EC multiple courses
string query_courses(string message){
    //msg out string
    string msg_out = "";
    //open file
    ifstream courses("ee.txt");
    string course_info; 
    //increment through EE classes in message, compare to ee.txt classes
    for(int i = 0; i <= message.length();i = i + 6){
        //printf("%i\n", i);
        int flag = 0;
        if(courses.is_open()){
            while(getline(courses,course_info)){
                
                //if class codes match,take in all category information
                if(course_info.substr(0,5) == message.substr(i,5)){
                    msg_out += message.substr(i,5) + ": " + course_info.substr(6,course_info.length() - 6) + "\n";
                    printf("Found course: %s\n", message.substr(i,5).c_str());
                    flag = 1;
                    break;
                }
            }
            if(!flag){
                msg_out += "Didn’t find the course: " + message.substr(i,5) + "\n";
                printf("Didn’t find the course: %s\n", message.substr(i,5).c_str());
            }
        }
        courses.clear();
        courses.seekg(0);
    }  
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

    //booting up message
    printf("serverEE is up and running using UDP on port %i\n", UDP_PORT);
    while(1){
        memset(buffer,0,102); //ensure buffer is cleared
        //wait for query from main
        recvfrom(udp_socket,buffer,102,0,(struct sockaddr *) &client_address, &client_length);
        string message(buffer); //convert to string
        string msg_out; //output sring
        //if 0 normal function - search 1 course 1 category
        if(message[0] == '1'){
            printf("The ServerEE received received a request from the Main Server about the %s of %s\n", message.substr(8,message.length()-8).c_str(), message.substr(2,5).c_str());
            msg_out = search_course(message.substr(2,message.length()-2));
        }
        //else extra credit - search multiple courses get all categoroes
        else if (message[0] == '2'){
            printf("The ServerEE received received a request from the Main Server about the following courses: %s\n", message.substr(2,message.length()-2).c_str());
            msg_out = query_courses(message.substr(2,message.length()-2));
        }
        //error occured
        else{
            printf("The ServerEE received received a invalid request from the Main Server, ERROR\n");
            msg_out = "Error occured\n";
        }
        //send response
        sendto(udp_socket,msg_out.c_str(),msg_out.length(),0,(struct sockaddr *) &client_address, client_length);
        printf("The ServerEE finished sending the response to the Main Server.\n");
    }
    return 0;
}