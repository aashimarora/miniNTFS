#include "common_header.h"


#define READ 1

#define WRITE 2

#define RENAME 3

#define DELETE 4

#define CLOSE 5


int displayMenu();

int sendRead(int, int);

int sendWrite(int, int,char *);

int sendRename(int, int);

int sendDelete(int, int);

int sendClose(int, int);
