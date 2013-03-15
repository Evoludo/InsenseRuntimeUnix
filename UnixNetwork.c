#include "UnixNetwork.h"

// bind and listen for incoming connections
int unicast_listen()
{
	// set up data structures
	int lsock; // listening socket
	struct sockaddr_in addr; // server address
	if(lsock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        {
		perror("Error: could not open socket to listen on");
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	// bind to port
	if(bind(lsock, (struct sockaddr*) &addr, sizeof(addr)) < 0) 
	{
		char err[50];
		sprintf(err, "Error: could not bind to port %d", PORT);
		perror(err);
	}

	// listen for incoming connections
	listen(lsock, CONQUEUESIZE);
}

// will guarantee message received if connection can be made and sustained; if connection drops, data is lost
void unicast_send(char* host, void* data, size_t size)
{
	// set up data structures
	int sock;
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("Error: could not open socket to send on");
	}
	struct sockaddr_in saddr; // server address
	socklen_t slength = sizeof(saddr); // size of the server's address
	struct hostent* shost_addr;

	// convert address string to IP address and set up address struct
	if((shost_addr = gethostbyname(host)) == NULL)
	{
		char err[50];
		sprintf(err, "Error: could resolve %s", host);
		perror(err);
	}
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = *(shost_addr->h_addr);
	saddr.sin_port = htons(PORT);

	// connect to listening Insense program
	if(connect(sock, (struct sockaddr*) &saddr, slength) < 0)
	{
		char err[50];
		sprintf(err, "Error: could connect to %s", host);
		perror(err);
	}

	// send data
	send(sock, data, size, 0);
}

// accept a connection from the queue, or block until one exists
void unicast_receive(int lsock, void* data)
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
	buffer = malloc(RECVBUFFSIZE * sizeof(char)); // going to free this at the end of the function anyway
	int i = 1;
	while(recv(datasock, buffer, RECVBUFFSIZE, 0) > 0)
	{
		temp = data;
		//data = DAL_alloc(RECVBUFFSIZE * i * sizeof(char), false);
		data = malloc(RECVBUFFSIZE * i * sizeof(char));
		
		if(i > 1)
		{
			memcpy(data, temp, RECVBUFFSIZE * (i - 1) * sizeof(char));
			//DAL_free(temp);
			free(temp);
		}

		memcpy(data + RECVBUFFSIZE * (i - 1) * sizeof(char), buffer, RECVBUFFSIZE);
		i++;
	}

}
