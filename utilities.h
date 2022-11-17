#ifndef UTILTIES_H
#define UTILTIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>

//creates any type of socket, and will bind if necessary
int create_socket(int type, bool bind_soc, int port);
#endif