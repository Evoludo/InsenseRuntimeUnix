/**
 * UnixNetwork.c
 * Socket operations.
 *
 * @author Andrew Bell
 *
 */


#include "UnixNetwork.h"

// bind and listen for incoming connections
int unicast_listen()
{
	// set up data structures
	int lsock; // listening socket
	struct sockaddr_in addr; // server address
	if((lsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
		perror("Error: could not open socket to listen on");
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	// Allow to bind if socket is in TIME_WAIT after closure
	int setting = 1;
	setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &setting, sizeof(setting));

	// bind to port
	if(bind(lsock, (struct sockaddr*) &addr, sizeof(addr)) < 0) 
	{
		char err[256];
		sprintf(err, "Error: could not bind to port %d", PORT);
		perror(err);
		return -1;
	}

	// listen for incoming connections
	listen(lsock, CONQUEUESIZE);
	fprintf(stderr, "Listening on port %d\n", PORT);
	return lsock;
}

// make sure all data is sent from buffer
int fullsend(int socket, void* buffer, int length)
{
	int totalsent = 0;
	int bytessent = 0;

	while(totalsent < length)
	{
		bytessent = send(socket, buffer + totalsent, length - totalsent, 0);
		if(bytessent < 0)
		{
			perror("Error: could not send on socket");
			return totalsent;
		}
		totalsent += bytessent;
	}
}

// will guarantee message received if connection can be made and sustained; if connection drops, data is lost
void unicast_send(char* host, void* data, size_t size)
{
	// set up data structures
	int sock;

	struct addrinfo hints, *servaddr, *curr_addr; // hints, server address, current address
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // use IPv4
	hints.ai_socktype = SOCK_STREAM;

	// convert address string to IP address and load into address struct
	int errval;
	if((errval = getaddrinfo(host, TOSTRING(PORT), &hints, &servaddr)) != 0)
	{
		char err[256];
		fprintf(stderr, "Error: %s\n", gai_strerror(errval));
		return;
	}

	
	// connect to listening Insense program
	for(curr_addr = servaddr; curr_addr != NULL; curr_addr = curr_addr->ai_next)
	{
		if ((sock = socket(curr_addr->ai_family, curr_addr->ai_socktype, curr_addr->ai_protocol)) < 0)
		{
			perror("Error: could create socket to send on");
			sleep(1);
			continue;
		}

		if(connect(sock, curr_addr->ai_addr, curr_addr->ai_addrlen) < 0)
		{
			perror("Error: could not connect");
			sleep(1);
			continue;
		}
	
		break;
	}

	if(curr_addr == NULL)
	{
		char err[256];
		sprintf(err, "Error: could connect to %s\n", host);
		perror(err);
		return;
	}

	// send data
	uint32_t sizen = htonl((uint32_t)size);
	fullsend(sock, &sizen, sizeof(uint32_t)); // send the size as a 32 bit integer, making sure it's portable
	fullsend(sock, data, size);

	char* dummy[32];
	int dr;
	while((dr = recv(sock, dummy, 32, 0)) != 0); // eat any data sent back until the socket is closed by the other side

	close(sock);

	freeaddrinfo(servaddr);
}

// accept a connection from the queue, or block until one exists; returns bytes received
int unicast_receive(int lsock, void** data, char** address)
{
	// set up data structures
	struct sockaddr_in caddr;
	socklen_t clength = sizeof(caddr);
	int datasock;
	void* buffer;

	// wait until connection received
	if((datasock = accept(lsock, (struct sockaddr*) &caddr, &clength)) < 0)
	{
		perror("Error: could not accept connection on socket");
		return -1;
	}

	// receive data
	buffer = malloc(RECVBUFFSIZE);
	bool beginning = true;
	int i = 0;
	int totalreceived = 0;
	int bytesreceived;
	int bytesremaining;
	int bytestocopy;
	int length;
	while((bytesreceived = recv(datasock, buffer, RECVBUFFSIZE, 0)) >= 0)
	{
		int offset = 0;
		
		if(beginning)
		{
			offset = 4;
			length = ntohl(*(int32_t*)buffer); // unserialise the size of the stream
			bytesremaining = length;
			if(*data != NULL)
			{
				free(*data);
			}
			*data = malloc(length);

			beginning = false;
		}

		if(bytesreceived == 0)
		{
			bytestocopy = bytesremaining <= (RECVBUFFSIZE - offset) ? bytesremaining : (RECVBUFFSIZE - offset);
		}
		else
		{
			bytestocopy = bytesreceived - offset;
		}
		memcpy(*data + totalreceived, buffer + offset, bytestocopy);
		totalreceived += bytestocopy;
		
		bytesremaining = length - totalreceived;

		if(bytesremaining == 0)
		{
			break;
		}
	}
	free(buffer);
	if(bytesreceived < 0)
	{
		perror("Error: could not read from socket");
		return 0;
	}

	char* addr_buff = malloc(clength);
	inet_ntop(AF_INET, &(caddr.sin_addr), addr_buff, clength);
	*address = addr_buff;

	close(datasock);

	return totalreceived;
}
