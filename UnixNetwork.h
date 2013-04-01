#ifndef UNIXNETWORK_H
#define UNIXNETWORK_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h> 

#define PORT 4120
#define STRINGEVAL(x) #x
#define TOSTRING(x) STRINGEVAL(x) // for using port with getaddrinfo()
#define CONQUEUESIZE 10
#define RECVBUFFSIZE 1024

// bind and listen for incoming connections
int unicast_listen();

// will guarantee message received if connection can be made and sustained; if connection drops, data is lost
void unicast_send(char* string, void* data, size_t size);

// accept a connection from the queue, or block until one exists
int unicast_receive(int lsock, void** data);

#endif
