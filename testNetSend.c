#include <stdio.h>
#include "UnixNetwork.h"

int main(int argc, char* argv[])
{
	FILE* fp;
	if((fp=fopen("test.send", "r"))==0)
	{
		perror("File");
		exit(1);
	}
	int size = 6;
	void* string = malloc(size);
	fread(string, size, 1, fp);
	unicast_send("localhost", string, size);
	return 0;
}
