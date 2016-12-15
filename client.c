/*ME : democlient.c

*	DESCRIPTION:Contains Code for a client that will send a string 	

*	to a server process and exits.

*	Invoke the Executable as a.out IPAddress PortNo string   

*****************************************************************************/

#include	<stdio.h>

#include	<stdlib.h>

#include	<unistd.h>

#include	<errno.h>

#include	<string.h>

#include	<sys/types.h>

#include	<sys/stat.h>

#include	<sys/socket.h>

#include	<netinet/in.h>

#include	<arpa/inet.h>

#include	<fcntl.h>

#include 	"client_header.h"



#define SIZE 50


int main( int C, char *V[] )

{

	int	sd,fd;
	int ent = 1;/* to check do-while cond*/

	int 	user_input;

	struct	sockaddr_in serveraddress;

	char    modtime[SIZE];/*to store mod time*/

	char perm[SIZE];/* to store permission*/

	char pathn[SIZE];/* to store pathname which is taken by user*/

	sd = socket( AF_INET, SOCK_STREAM, 0 );

	if( sd < 0 ) {

		perror( "socket" );

		exit( 1 );

	}

	if (V[1] == NULL ) {

	printf ("PL specfiy the server's IP Address \n");

	exit(0);

	}

	if (V[2] == NULL ) {

	printf ("PL specify the server's Port No \n");

	exit(0);

	}


	memset( &serveraddress, 0, sizeof(serveraddress) );

	serveraddress.sin_family = AF_INET;

	serveraddress.sin_port = htons(atoi(V[2]));//PORT NO

	serveraddress.sin_addr.s_addr = inet_addr(V[1]);//ADDRESS

	if (connect(sd,(struct sockaddr*)&serveraddress,

			sizeof(serveraddress))<0)

	{

		printf("Cannot Connect to server");

		exit(1);

	}
	
	memset(perm,0,sizeof(perm));

	receiveData(sd,perm);/* rece dir name and permission*/

	printf("%s\n",perm);

	char *pt=NULL;/* to store dir name*/

	pt = strtok(perm, " ");

	printf("%s\n",pt);

	printf("len %d\n",strlen(pt));


	do
	{
		memset(pathn,0,25);
	
		ent = 1;	

		printf("plz enter pathname\n");

		fgets(pathn, SIZE, stdin);

		int len = strlen(pathn);

		printf("len %d\n",len);

		if(strncmp(pt,pathn,strlen(pt)))
		{
			printf("enter again\n");

			ent = 0;
		}

	}while(ent ==0);

	sendData(sd,pt);/* path name to server*/

	memset(modtime, 0, sizeof(modtime));

	receiveData(sd,modtime);/* modified tym*/

	printf("%s\n",modtime);

	displayMenu();

	scanf("%d", &user_input);

	sendRequests(user_input,sd,modtime);/* modtime parameter send bcz i want to use it in write file for comapring with getlastmodtym*/

	shutdown(sd, SHUT_RDWR);	

	return 0;

}

