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
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	// bind to port
	if(bind(lsock, (struct sockaddr*) &addr, sizeof(addr)) < 0) 
	{
		char err[256];
		sprintf(err, "Error: could not bind to port %d", PORT);
		perror(err);
	}

	// listen for incoming connections
	listen(lsock, CONQUEUESIZE);
	return lsock;
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
		fprintf(stderr, "Error: %s", gai_strerror(errval));
		exit(1);
	}

	
	// connect to listening Insense program
	for(curr_addr = servaddr; curr_addr != NULL; curr_addr = curr_addr->ai_next)
	{
		if ((sock = socket(curr_addr->ai_family, curr_addr->ai_socktype, curr_addr->ai_protocol)) < 0)
		{
			perror("Error: could create socket to send on");
			continue;
		}

		if(connect(sock, curr_addr->ai_addr, curr_addr->ai_addrlen) < 0)
		{
			perror("Error: trying next address");
			continue;
		}
	
		break;
	}

	if(curr_addr == NULL)
	{
		char err[256];
		sprintf(err, "Error: could connect to %s", host);
		perror(err);
	}

	// send data
	/*int i;
	for(i = 0; i < size; i += 1024)
	{
		send(sock, data + i, (size - i > RECVBUFFSIZE ? RECVBUFFSIZE : size % RECVBUFFSIZE), 0);
	}*/

	send(sock, data, size, 0);

	close(sock);
	freeaddrinfo(servaddr);
}

// accept a connection from the queue, or block until one exists; returns bytes received
int unicast_receive(int lsock, void** data)
{
	// set up data structures
	struct sockaddr_in caddr;
	socklen_t clength = sizeof(caddr);
	int datasock;
	void* buffer;
	void* temp;

	// wait until connection received
	if((datasock = accept(lsock, (struct sockaddr*) &caddr, &clength)) < 0)
	{
		perror("Error: could not accept connection on socket");
	}

	// receive data
	buffer = malloc(RECVBUFFSIZE); // going to free this at the end of the function anyway
	int i = 0;
	int bytesreceived;
	int totalreceived = 0;
	while((bytesreceived = recv(datasock, buffer, RECVBUFFSIZE, 0)) > 0)
	{
		temp = *data;
		//data = DAL_alloc(RECVBUFFSIZE * i false);
		*data = malloc(bytesreceived + RECVBUFFSIZE * i);
		
		if(i > 0)
		{
			memcpy(*data, temp, RECVBUFFSIZE * i);
			//DAL_free(temp);
			free(temp);
		}

		memcpy(*data + RECVBUFFSIZE * i, buffer, bytesreceived);
		totalreceived += bytesreceived;
		i++;
	}
	return totalreceived;

}
