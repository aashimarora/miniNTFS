#include "server_header.h"





fdList* allocateFd() 

{

	fdList* newnode = NULL;

	newnode = malloc(sizeof(fdList));

 	if (NULL == newnode)

	{

		printf(" malloc failure");

	}

	return newnode; 

		

}



int insertFd(int fd, char* pathname) 

{

	 fdList* newnode = NULL;

	 newnode = allocateFd();

	 newnode->fd = fd;

	 strncpy(newnode->pathname ,pathname, strlen(pathname)) ;

	 if(NULL == head)

	 {

	    head = newnode;

	 }

	 else

	 {

	     fdList* temp = head;

	     while (NULL != temp->next)

	     {

	         temp = temp->next;

    		 temp->next = newnode;

	     }

	 }

}



int deleteFd(int fd)

{

	fdList*  del = NULL;

	if (NULL ==  head)

	{

		printf("underflow");

		return FAILURE;

	}

	else if (head->fd == fd)

	{

		del = head;

		head = head->next;

	}

	else

	{	

		fdList* temp = head;

		fdList* save = NULL;

        	while(NULL != temp)

          	{

			save = temp;

        	 	if(temp->fd == fd)

               		{

				del = temp;

				save->next = temp->next;

				break;

              		}

		}

    		free(del);

   		return SUCCESS;



	}

}





int freeFd()

{

	fdList* temp = head;

	while (NULL != temp)

	{

		fdList* del = temp;

		temp = temp->next;

		free(del);

	}

	return SUCCESS;

}




