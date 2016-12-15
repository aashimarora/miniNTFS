/*ME : threadedfs.c

*	DESCRIPTION:Contains Code for a file server , that will accept a filename

*	from a file client process  read that file and sends it over socket  

*	to the client process .(Shows a typical THREADED SERVER )

*	Invoke the Executable as a.out   

*       You may use the same fileclient, which was used for iterative demo.

******************************************************************************/

#include	<stdio.h>

#include	<stdlib.h>

#include	<errno.h>

#include	<string.h>

#include	<sys/types.h>

#include	<sys/socket.h>

#include	<netinet/in.h>

#include	<arpa/inet.h>

#include	<sys/wait.h>

#include	<fcntl.h>

#include	<pthread.h>

#include	<unistd.h>

#include	<signal.h>

#include 	"server_header.h"

#include	"export.h"



#define MYPORT 30079

fdList* head = NULL;

int sd; /* for signal handler but ist close client server then use cntl + c to terminate server*/

void signal_handler(int signum)
{
	close(sd);
	exit(0);
}

void* serveClient(void* connfd) ;
int authenticateClient(char *, char**);/* to check auth for client*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main(void) {
        
        /* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();

        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        else if (pid >  0) {
	
		printf("process_id of child process %d \n", pid);

		exit(EXIT_SUCCESS);
	}

       /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        

        
        /* Change the current working directory */
        //if ((chdir("/")) < 0) {
                /* Log the failure */
        //        exit(EXIT_FAILURE);
        //}
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

	int fd = open("/dev/null/",O_RDWR);
	dup(fd);
	dup(fd);
        
	int	connfd,retbind;
	int 	ret_pthread = 0;
	int ret_code = 0;
	
	FILE* errorfd = fopen("errors","w+");

	FILE* logfd = fopen("log","w+");
	
	if(NULL == logfd)
	{
		fprintf(errorfd,"%s","Error creating log file");
	}

	if(NULL == errorfd)
	{
		fprintf(errorfd,"%s","Error creating errors log file");
	}

	struct	sockaddr_in serveraddress, cliaddr;

	socklen_t len;

	pthread_t th1;

	char buf[SIZE];

	char *out_buf = NULL;/* to store dir n permission*/

	signal(SIGINT, signal_handler);

	sd = socket( AF_INET, SOCK_STREAM, 0 );

	if( sd < 0 ) {

		fprintf(errorfd, "%s", "socket not opened\n");

		exit( 1 );

	}

	memset( &serveraddress, 0, sizeof(serveraddress) );

	serveraddress.sin_family = AF_INET;

	serveraddress.sin_port = htons(MYPORT);//PORT NO

	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS

	retbind=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));

	if(-1==retbind)

	{
		fprintf(errorfd,"%s", "Bind Fails\n");

		exit(1);
	}

	listen(sd,5);

        /* Daemon-specific initialization goes here */
        
        /* The Big Loop */
	
	/*Beginning of the Main Server Processing Loop*/

	for(;;){

		fprintf(logfd,"%s","I am waiting-----Start of Main Loop\n");

		len=sizeof(cliaddr);

		connfd=accept(sd,(struct sockaddr*)&cliaddr,&len);

		if ( connfd < 0)

		{

			if (errno == EINTR) 

			fprintf(errorfd,"%s","Interrupted system call ??");

			continue;

		}

		/* write func for auth*/
		memset(buf,0,100);

		fprintf(logfd,"%s","Connection from %s\n",

		inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));
		fprintf(logfd,"%s","ready to auth\n");
		
		ret_code = authenticateClient(buf,&out_buf); /* to call auth_cli func*/

		if (ret_code == 0)
		{
			out_buf = "not found";
		}

		fprintf(logfd,"%s%s\n","auth compl",out_buf);

		sendData(connfd,out_buf);/* send dir n perm to client*/

		ret_pthread=pthread_create(&th1,NULL,&serveClient,(void*)&connfd);		

		fclose(errorfd);
		fclose(logfd);
			

	}

	close(sd);
	return 0;
}



void* serveClient(void* connfd) 

{

	char path[SIZE];	
	int conn = 0;

	conn = *((int*)connfd);

	//clientInfo * cl = info;

	char buf[SIZE];

	char last_modtime[SIZE];

//	memset(buf, 0, sizeof(buf));

	memset(path, 0, sizeof(path));

	memset(last_modtime, 0, sizeof(last_modtime));

	pthread_detach(pthread_self());

	/*string IPAddress = cl->client_IPaddr;

 	auth* authInfo = CALL authenticateClient(IPAddress);

	if (NULL != authInfo) 

	{

	INITIALIZE Buffer with authInfo;

	send buf to client;

	}

	

	else

	{

	send error message to client;

	exit

      	}

	*/

	memset(buf, 0, sizeof(buf)); 

	receiveData(conn, path);


	/*char* valid = strtok(path,'/');

	int retval = compare(valid,authInfo->dirname);

	CHECK if(0 equals retval)

	{

	*/

		fileInfo* fi = mount(path);

		fi->last_mod_time[strlen(fi->last_mod_time) - 1] = '\0';

		sprintf(last_modtime, "%s", fi->last_mod_time);

		sendData(conn,last_modtime); // Send last modification time

		receiveData(conn, buf);

		handleRequests(conn, buf, fi); 

		shutdown(conn, SHUT_WR);	

		close(conn);

	/*}*/

	

}


/* auth func*/

int authenticateClient(char * in, char ** out)
{
	
	int index;
	int cmp_ret = 0;
	for(index = 0; index < MAX_LIST; index += 1)

      	{

      		/* Comparing input string with first element of the list */

   

      		cmp_ret = strncmp(in, morse_list[index][0],strlen(in));

      		if(0 == cmp_ret) /* Checking for a match */

      		{
      			/* Assigning the morse code of matched string constant to the output variable */
      			*out= morse_list[index][1];

      			return 1;

      		}
		
	}
	return 0;
}	
