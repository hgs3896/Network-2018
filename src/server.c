/*
 * Last Modified Date: 2018.12.09
 * ID : 2017030473
 */
#include "essential.h"
 
ssize_t fillData(int sd, void *data, size_t datasize, const char* errmsg);

int main(int argc, char *argv[]){
	int fd, serverSD, clientSD;
	int pid;
	char buf[BUFFER_SIZE];
	char metadata[METADATA_SIZE];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clAddrLen;
	
	// Create listen socket
	if((serverSD = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	// Bind listen socket to the local address and port
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(DEFAULT_PORT);

	if(bind(serverSD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	// Listen to connection request
	if(listen(serverSD, 5) == -1){
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	clAddrLen = sizeof(clientAddr);

	// Handle the connection
	while(true)
	{
		if((clientSD = accept(serverSD, (struct sockaddr*)&clientAddr, &clAddrLen)) == -1)
		{
			perror("accept error");
			continue;
		}

		fillData(clientSD, metadata, sizeof(metadata),
				"fail to get the metadata");
		size_t sz;
		char filename[20];
		sscanf(metadata, "%lu %s", &sz, filename);

		if((fd = open(filename, O_WRONLY | O_CREAT, 0644)) == -1)
		{
			perror("fail to read the metadata");
			exit(EXIT_FAILURE);
		}

		lseek(fd, 0, SEEK_SET);

		size_t n = 0, rn;
		
		// Read the data from the socket
		while(n < sz)
		{
			rn = fillData(clientSD, buf, sizeof(buf),
					"error while receiving the file");

			// Count how many bytes get received.
			n += rn;

			// Write the data to the file
			write(fd, buf, rn);
		}
		
		close(fd);
		close(clientSD);

		printf("\tThe file \"%s\" consisting %ld bytes was received %s.\n",
						filename, n, n == sz ? "perfectly" : "poorly");
	}

	return 0;
}

ssize_t fillData(int sd, void *data, size_t datasize, const char* errmsg){
	// Clear the metadata
	memset(data, 0, datasize);

	ssize_t bytesRecv, pos = 0;
	// Read the File Meta Data
	while(pos < datasize){
		bytesRecv = read(sd, data + pos, datasize - pos);
		if(bytesRecv == -1)
		{
			perror(errmsg);
			exit(EXIT_FAILURE);
		}
		// EOF
		if(bytesRecv==0)
			break;
		pos += bytesRecv;
	}
	return pos;
}
