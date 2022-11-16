#include <sys/socket.h>

//creates any type of socket, and will bind if necessary
int create_socket(int type, bool bind, int port){
    int new_socket;
    if((new_socket = socket(AF_INET,type,0))< 0){
        printf("Error could not create socket\n");
        exit(1);
    }

    //if bind is true bind port and address to socket
    if(bind){
      struct sockaddr_in sock_address;
      sock_address.sin_family  = AF_INET;
      sock_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
      sock_address.sin_port = htons(port); // htons converts to network byte order(big endian)

      //bind port no and address to socket
      if(bind(udp_socket,(struct sockaddr *)&udp_address, sizeof(udp_address))<0){
          printf("Error could not bind\n");
          exit(1);
      }
    }
    return new_socket;
}
