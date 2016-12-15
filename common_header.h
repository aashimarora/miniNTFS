#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <time.h>

#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <sys/socket.h>

#include <pthread.h>


#define SIZE 50 

#define SUCCESS 1

#define FAILURE 0



int sendData(int, char*);

int receiveData(int, char*);
