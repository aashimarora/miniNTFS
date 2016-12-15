#include "server_header.h"



int searchList(char* pathname)

{

	fdList* temp = head;

	int retval;

	int fd = -1; 

  	while(NULL != temp) 
	{

      		retval = strncmp(temp->pathname, pathname,strlen(pathname));

     		if( 0 ==  retval)
		{
        		fd = temp->fd;

			printf("we found\n");

        		break;
		}

	}

	return fd;

}

fileInfo* mount(char* pathname) 

{

	fileInfo* fs = NULL;

	fs = malloc(sizeof(fileInfo));

	memset(fs, 0, sizeof(fs));

 	char* modTime;

 	int fd = searchList(pathname);

   	if (-1 == fd)
	{

            fd = open(pathname,O_RDWR, S_IRWXU);
	    if (-1 == fd)
	    {
		perror("open");
		exit(FAILURE);
	    }
            insertFd(fd,pathname);

        }

 	fs->fd = fd;

	memset(fs->pathname,0,sizeof(fs->pathname));

	strncpy(fs->pathname, pathname, strlen(pathname));

 	modTime = getLastModifiedTime(fd);

	if(NULL != modTime) 

	{

		strncpy(fs->last_mod_time, modTime, strlen(modTime));

	}

 	return fs;

}





char* getLastModifiedTime(int fd) 

{

	struct stat sb;

	char* time = NULL;

	int retval = fstat(fd,&sb);

	if(0 == retval) 

	{

		time = ctime(&(sb.st_mtime));

	}

	else

	{

		perror(" error for fstat");

	}

 	return time;

} 


int handleRequests(int sd, char* command, fileInfo* fs) 

{

	int fd = fs->fd;

	int code = atoi(strtok(command, " "));

	char* newname;

	char* pathname = fs->pathname;

	switch(code) 

	{

		case 1: readFile(sd, fd);

			break;

		case 2: writeFile(sd, fs);

			break;

		case 3: newname = strtok(NULL, " ");

			renameFile(sd, pathname, newname);

			break;

		case 4: deleteFile(sd, pathname);

			break;

		case 5: umount(sd, fd);

        	        break;

	}

}



int readFile(int sd, int fd)

{

	char buf[SIZE];

	memset(buf, 0, sizeof(buf));

	lseek(fd,0,SEEK_SET);

 	int c = read(fd, buf, sizeof(buf));

	if( c > 0)

	{

		sendData(sd, buf);

	}

	return SUCCESS;

}



int writeFile(int sd, fileInfo* fs)

{
	char * modTime;

	char buf[SIZE];

	int ret;

	int fd = fs->fd;

	int status = readFile(sd,fd);

	if(!status)
	{
		printf("Error in reading file");	
		exit(FAILURE);
	}
	memset(buf,0,sizeof(buf));

	receiveData(sd,buf);/* req for last mod tym*/

	modTime = getLastModifiedTime(fd);/* call func n store last mod tym in buf*/

	sendData(sd,modTime);/*  send last mod tym*/

	memset(buf,0,sizeof(buf));

	receiveData(sd,buf);/* client response*/

	if(!strncmp(buf,"not save",strlen(buf)))
	{
		printf("do not save\n");
	}
	else
	{
		close(fd);

		fd = open(fs->pathname,O_RDWR|O_TRUNC, S_IRWXU);

		ret = write(fd,buf,strlen(buf));

		close(fd);

		fd = open(fs->pathname,O_RDWR, S_IRWXU);
			
	}
	return SUCCESS;

}

int renameFile(int sd, char* oldname, char* newname)
{}

int deleteFile(int sd, char* path)
{}

int umount(int sd, int fd)
{}
