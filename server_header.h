#include "common_header.h"



typedef struct 

{

	char pathname[SIZE];

	char permissions[SIZE];

	

}auth;





typedef struct

{

	int fd;

	char pathname[SIZE];

        char last_mod_time[SIZE];



}fileInfo;



typedef struct fdList

{

	int fd;

	char pathname[SIZE];

	struct fdList*  next;

	

}fdList;





extern fdList* head;





fileInfo* mount(char*);

int searchList(char*);

char* getLastModifiedTime(int);

int handleRequests(int, char*, fileInfo*);

int readFile(int, int);

int writeFile(int, fileInfo*);

int renameFile(int, char*, char*);

int deleteFile(int, char*);

int umount(int, int);

fdList* allocateFd();

int insertFd(int, char*);

int deleteFd(int);

int freeFd();
