#include <stdio.h>
#include "UnixNetwork.h"

int main(int argc, char* argv[])
{
	FILE* fp;
	if((fp=fopen("test.recv", "w"))==0)
	{
		perror("File");
		exit(1);
	}
	int socket = unicast_listen();
	void** string = malloc(sizeof(void*));
//	while(1)
//	{
		char* address;
		int n = unicast_receive(socket, string, &address);
		if(*string != 0)
		{
			//(*(char**)string)[n] = '\0';
			//printf("%s", *(char**)string);
			printf("%d bytes received from %s\n", n, address);
			fwrite(*string, n, 1, fp);
		
		}
		else
		{
			printf("No data received.\n");
		}
//	}
	return 0;
}
