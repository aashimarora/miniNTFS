#include "client_header.h"

int displayMenu()

{

	printf("------- MENU--------\n");	 

   	printf(" 1: Read \n");

   	printf(" 2: Write \n");

	printf(" 3: Rename \n");

  	printf(" 4: Delete \n");

 	printf(" 5: Close \n");

	printf(" Enter your choice : ");



	return SUCCESS;

}





void sendRequests(int code, int sd, char *modtime) 

{

	switch(code)

	{

		case 1: sendRead(READ, sd);

         	        break;

		case 2: sendWrite(WRITE, sd,modtime);

			break;

	/*	case 3: sendRename(RENAME, sd);

                 	break;

      		case 4: sendDelete(DELETE, sd);

                   	break;

      		case 5: sendClose(CLOSE, sd);

                   	break;

	*/

    	}

}





int sendRead(int code, int sd) 
{	

	char buffer[SIZE];	

	char response[SIZE];

	memset(buffer,0,sizeof(buffer));

	sprintf(buffer, "%d", code);

	buffer[strlen(buffer)] = ' ';

	sendData(sd,buffer);

	//set timeout duration

	

	memset(response,0,sizeof(response));

	receiveData(sd,response);

	/*

	REPEAT UNTIL timeout 

	{

		CHECK if timeout has occurred

		{

			Force CLOSE

			Restart Client

			Send Requests Again

			Receive data in buffer

        	} 

    

	}

	*/

	printf("\n Data : ");

	printf("%s", response);



	return SUCCESS;

}

int sendWrite(int code, int sd, char *modtime)
{
	srand(time(NULL));
	
	long int random = rand();

	char filename[SIZE];

	char tempfile[SIZE];

	char buffer[SIZE];

	char response[SIZE];

	char remove[SIZE];

	int ret = 0;

	int ret1 = 0;

	int fd1=0;  /* to store filee desp for new file*/

	memset(buffer,0,sizeof(buffer));

	sprintf(buffer,"%d",code);

	buffer[strlen(buffer)] = ' ';

	sendData(sd,buffer);/* send write code to server*/

	memset(response,0,sizeof(response));

	receiveData(sd, response);/* recev content of given file from user*/

	printf("%s\n",response);

	memset(filename,0,sizeof(filename));

	sprintf(filename,"%s%ld","temp_",random);

	fd1 = open(filename,O_RDWR|O_CREAT,S_IRWXU); /* create and open while in w a r mode*/

	if ( fd1 < 0 )
	{

		printf("File opening failed!\n");

		exit(0);

	}

	memset(buffer,0,sizeof(buffer));

	ret=write(fd1,response,strlen(response));/* write content in file*/

	close(fd1);

	printf("close file\n");

	memset(tempfile,0,sizeof(tempfile));

	strcpy(tempfile,"vi ");

	strncat(tempfile,filename,strlen(filename));

	system(tempfile);/* open file for user*/
	
	printf("req for mod time\n");

	memset(response,0,sizeof(response));

	strcpy(response,"send");

	sendData(sd,response);/* req for last mod tym*/

	memset(response,0,sizeof(response));/* at this point got segmentation fault*/

	receiveData(sd,response);/* recev last mod tym*/

	printf("get modtym\n");

	if (!strncmp(modtime,response,strlen(modtime)))/* com with earlier mod tym*/
	{
			fd1 = open(filename ,O_RDWR|O_CREAT,S_IRWXU); /* create and open while in w a r mode*/
			ret1=read(fd1,buffer,sizeof(buffer));

			close(fd1);
	}
	else
	{
		strcpy(buffer,"not save");
	}
	memset(remove,0,sizeof(remove));

	strcpy(remove,"rm ");

	strncat(remove,filename,strlen(filename));

	system(remove);/* remove file from client end*/

	sendData(sd,buffer);/* send data to server*/

	return SUCCESS;

}

