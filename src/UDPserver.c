/*****************************************************************************
 * Ricky Haynes							    	     *
 * UDPserver.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program starts an UDP server. 					     *
 *									     *
 *****************************************************************************/
#include "UDP.h"

int UDPserver()
{
	int sd, port = SERVER_UDP_PORT, n, data_size;
	char buf[MAXLEN], nbuf[MAXLEN], tbuf[MAXLEN], complete[1];
	int fileNum, seqNum, txtSize, fileSize, fileTotal = 0, newFileTot = 0;
	int fileCount = 0, curSeq = 1, namLen = 0;
	long int size;
	unsigned int client_len;
	char *name = malloc((HEAD-16)*sizeof(char));
	char *ch = malloc(MAXLEN*sizeof(char));

	
	struct sockaddr_in server, client;

	// Create a datagram socket
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		
		fprintf(stderr, "can't create socket\n");
		exit(1);
	}
	// Bind address to socket
	memset((char *)&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	while (1) // While receiving 
	{
		printf("Waiting...\n");	
		size = sizeof(buf);
		memset(buf, '\0', size);
		client_len = sizeof(client);
		if ((n = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr *)&client, &client_len)) < 0)
		{

			fprintf(stderr, "Can't receive datagram\n");
			exit(1);
		}
		if(!strcmp(buf, "kill")) // Checks if end of transmission
			break;
		size = sizeof(name);
		memset(name, '\0', size);
		getData(buf, &fileNum, &seqNum, &txtSize, &fileSize, name); // Breaks apart payload
		namLen = strlen(name) + 2;
		
		if(fileNum == fileCount && curSeq == seqNum)
		{
						
			if(fileTotal == 0) // Adds file name to cat file before text is inputted
			{
				strcat(name, ":\n\n");
				namLen = strlen(name);
				memcpy(&nbuf[newFileTot], name, namLen);	
				newFileTot += namLen;
			}
			
			curSeq++;			
			fileTotal += txtSize;
					
			memcpy(&nbuf[newFileTot], remHead(buf, txtSize), txtSize); // Adds text to cat file
			newFileTot += txtSize; // Total size of cat file
			
			if(fileTotal >= fileSize)
			{
				complete[0] = 'c'; // File is completed
				fileCount++;
				fileTotal = 0;
				memcpy(&nbuf[newFileTot], "\n\n", 2); // Adds spacing at the end of the final text
				newFileTot += 2;
				curSeq = 1;
			} else
				complete[0] = 'n'; // File is incompleted
			
			if (sendto(sd, complete, n, 0, (struct sockaddr *)&client, client_len) != n)
			{

				fprintf(stderr, "Can't send datagram\n");
				exit(1);
			}
		} else
		{
			complete[0] = 'f';	// Incorrect file		
			if (sendto(sd, complete, n, 0, (struct sockaddr *)&client, client_len) != n)
			{

				fprintf(stderr, "Can't send datagram\n");
				exit(1);
			}
		}
		
		
		nbuf[newFileTot] = '\0';
		printf("Payload recieved.\n\n");
	}
	// Preparing to send cat file back
	char *pch = malloc(MAXLEN*sizeof(char));
	size = sizeof(pch);
	memset(pch, '\0', size);
  	fileSize = strlen(nbuf);
  	pch = strtok (nbuf,"\n"); // Tokenizes cat file by line
	int randLine;
	int count;
	seqNum = 0;
  	while (pch != NULL)
  	{
   		randLine = (rand() % 3) + 1; // Random number of lines
		size = sizeof(ch);
		memset(ch, '\0', size);
		count = 0;
		for( int i=0;i<randLine;i++) // Gets x many lines of cat file
		{
			if(pch == NULL)
				break;			
			memcpy(&ch[count], pch, strlen(pch));
			count += strlen(pch);
			ch[count++] = '\n';	
			
	    		pch = strtok (NULL, "\n");
			
			
		}
		ch[count] = '\0';

		txtSize = strlen(ch);
		ch = addHeader(ch, 0, seqNum, txtSize, fileSize, "CatFile"); // Adds header to payload
		size = sizeof(tbuf);
		memset(tbuf, '\0', size);
		strcpy(tbuf, ch);
	
		while(1) // While packet is not received
		{
			data_size = strlen(tbuf);
			
			// transmit payload
		    	if(sendto(sd, tbuf, data_size, 0, (struct sockaddr *)&client, client_len) == -1)
			{
				fprintf(stderr, "sendto error\n");
				exit(1);
			}
		
			// receive response payload
			if (recvfrom(sd, buf, MAXLEN, 0 , (struct sockaddr *)&client, &client_len) < 0)
			{
				fprintf(stderr, "recvfrom error\n");
				exit(1);
			}
			if(buf[0] != 'f') // If the correct packet was received, break
				break;
		}
		seqNum++;
  	}
	char kill[5] = "kill\0";
	// Once the total cat file is sent, send termination
	if(sendto(sd, kill, data_size, 0, (struct sockaddr *)&client, client_len) == -1)
		{
			fprintf(stderr, "sendto error\n");
			exit(1);
		}
	
	return 0;
}



