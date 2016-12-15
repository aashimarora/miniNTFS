#include "common_header.h"

int sendData(int sd, char *sent)
{
        int n , byteswritten=0 , written ;

        char buffer[SIZE];

        strcpy(buffer , sent);

	strncat(buffer, "*", 1);

        n=strlen(buffer);

        while (byteswritten < n)
        {
                written = write(sd , buffer+byteswritten,(n-byteswritten));

                byteswritten += written;
        }

        printf("String  : %s sent\n",buffer);

        return SUCCESS;

}

int receiveData(int sd, char* received)
{
	int flag = 0;

	int retval = 0;

	char temp ;

	while((retval = read(sd,&temp,1))> 0)
	{
		if(temp == '*')
		{
			temp = '\0';
			break;
		}
		strncat(received,&temp,1);
	}
		
	strncat(received,&temp,1);
	
	return SUCCESS;

}
