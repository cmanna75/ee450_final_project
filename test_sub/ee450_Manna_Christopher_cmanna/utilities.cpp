#include "utilities.h"
//creates any type of socket, and will bind if necessary
int create_socket(int type, bool bind_soc, int port){
    int new_socket;
    if((new_socket = socket(AF_INET,type,0))< 0){
        printf("Error could not create socket\n");
        exit(1);
    }

    //if bind is true bind port and address to socket
    if(bind_soc){
      struct sockaddr_in soc_address;
      soc_address.sin_family  = AF_INET;
      soc_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
      soc_address.sin_port = htons(port); // htons converts to network byte order(big endian)

      //bind port no and address to socket
      if(bind(new_socket,(struct sockaddr *)&soc_address, sizeof(soc_address))<0){
          printf("Error could not bind\n");
          exit(1);
      }
    }
    return new_socket;
}
//creates recipient address to be used.
sockaddr_in create_address(int port){
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    address.sin_port = htons(port); // htons converts to network byte order(big endian)
    return address;
}