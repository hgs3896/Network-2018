/*
 * Last Modified Date: 2018.12.09
 * ID : 2017030473
 */
#include "essential.h"

int main(int argc, char *argv[]){
	char *ip_addr = DEFAULT_IP;
	int port = DEFAULT_PORT;
	char *file_path = DEFAULT_FILENAME;

	int csd, fd;
	int pid;
	char sendBuf[BUFFER_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t clAddrLen;

	if(argc <= 1){
		puts("Missing Information");
		exit(EXIT_FAILURE);
	}

	if(argc >= 2)
		file_path = argv[1];

	if(argc >= 3)
		ip_addr = argv[2];

	if(argc >= 4)
		port = atoi(argv[3]);

	printf("Server Connection Info\n"
		   "\t%s:%d\n"
		   , ip_addr, port);

	printf("File Info\n"
		   "\t: %s\n"
		   , file_path);

	// Open an existing file for transferring
	if((fd = open(file_path, O_RDONLY)) == -1){
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	
	// Create a socket to communicate with the server.
	if((csd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	// Set the server infomation into 'serverAddr'
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if(inet_pton(AF_INET, ip_addr, &serverAddr.sin_addr) == -1){
		perror("Invalid Address error");
		exit(EXIT_FAILURE);
	}

	// Connect to the server.
	if(connect(csd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("Connect error");
		exit(EXIT_FAILURE);
	}

	// Load the file stat.
	struct stat st;
	fstat(fd, &st);
	ssize_t n, len, sz = st.st_size;

	const char* fname = file_path;
	while(*fname++);
	while(--fname != file_path){
		if(*fname == '/'){
			++fname;
			break;
		}
	}
	
	memset(sendBuf, 0, sizeof(sendBuf));
	sprintf(sendBuf, "%ld\n%s", sz, fname);
	write(csd, sendBuf, METADATA_SIZE);

	n = 0;
	while(true)
	{
		len = read(fd, sendBuf, BUFFER_SIZE);
		
		if(len == -1){
			perror("File read error");
			exit(EXIT_FAILURE);
		}

		if(len == 0){
			break;
		}

		if(write(csd, sendBuf, len) == -1){
			perror("Send error");
			exit(EXIT_FAILURE);
		}

		n += len;
	}

	close(fd);
	close(csd);

	printf("%ld bytes sent %s\n"
		, n, n == sz ? "perfectly" : "poorly");

	return 0;
}
