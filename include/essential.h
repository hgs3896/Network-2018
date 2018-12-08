#ifndef _ESSENTIAL_H_
#define _ESSENTIAL_H_

#include <stdio.h>      /* printf, scanf */
#include <sys/types.h>  /* struct sockaddr_in */
#include <sys/socket.h> /* socket, bind */
#include <sys/stat.h>   /* struct stat */
#include <arpa/inet.h>  /* htons */
#include <unistd.h>     /* fork, close */
#include <string.h>     /* memset */
#include <stdlib.h>     /* exit */
#include <fcntl.h>      /* O_RDONLY */

typedef enum {false, true} bool;
#define DEFAULT_IP       "127.0.0.1"
#define DEFAULT_PORT     12345
#define DEFAULT_FILENAME "file.txt"
#define BUFFER_SIZE      512 
#define METADATA_SIZE    64

#endif
