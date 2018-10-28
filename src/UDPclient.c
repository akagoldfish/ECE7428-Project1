/*****************************************************************************
 * Ricky Haynes							    	     *
 * UDPclient.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program starts an UDP client to send ten files to an UDP server to be     *
 * concatenated into one file. 					             *
 *									     *
 *****************************************************************************/
#include "UDP.h"

char *listdir(const char *dir, int *fc, int fileNumber);
char *getLine(char *folder, int fileComplete[MAXFILE], int lineN[MAXFILE], int seq[MAXFILE], int charN[MAXFILE], int f[3]);
long delay (struct timeval t1, struct timeval t2);

long int UDPclient(char *host, char *folder)
{
	int data_size = DEFLEN, port = SERVER_UDP_PORT;
	int f[3], i, sd, fileComplete[MAXFILE], lineN[MAXFILE], seqNum[MAXFILE], charN[MAXFILE], seq;
	char rbuf[MAXLEN], sbuf[MAXLEN], nbuf[MAXLEN], complete[1];
	unsigned int  server_len;
	int fileNum, sqNum, txtSize, fileSize, fileTotal = 0, newFileTot = 0;
	char *name = malloc((HEAD-16)*sizeof(char));

	struct hostent *hp;
	struct sockaddr_in server;
	struct timeval start, end;

	time_t t;

	srand((unsigned) time(&t));

	for(i=0; i<MAXFILE; i++) // Fills arrays with 0's
	{
		fileComplete[i] = 0;
		lineN[i] = 0;
		seqNum[i] = 0;
		charN[i] = 0;
	}

	// Create a datagram socket
	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		fprintf(stderr, "Can't create socket\n");
                exit(1);
	}

	// Store server's info
	memset((char *)&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
        server.sin_port = htons(port);
	if((hp = gethostbyname(host)) == NULL)
	{
		fprintf(stderr, "Can't get server's IP address\n");
		exit(1);
	}
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr_list[0], hp->h_length);
	if(data_size > MAXLEN)
	{
		fprintf(stderr, "Data is to large\n");
		exit(1);
	}

	char *ch;
	ch = malloc(MAXLEN*sizeof(char));
	int TCF = 0;
	i=0;	
	// Starts transmission
	printf("Sending...\n");

	gettimeofday(&start, NULL); // start delay measure

	while(TCF < MAXFILE) // While all of the files have not been fully sent
	{
		ch = getLine(folder, fileComplete, lineN, seqNum, charN, f); // Gets random number of lines from a random file
		memset(sbuf, '\0', sizeof(sbuf));
		strcpy(sbuf, ch);
		
		data_size = strlen(sbuf);
		// transmit data
		server_len = sizeof(server);
	    	if(sendto(sd, sbuf, data_size, 0, (struct sockaddr *)&server, server_len) == -1)
		{
			fprintf(stderr, "sendto error\n");
			exit(1);
		}
		
		// receive response data
		if (recvfrom(sd, rbuf, MAXLEN, 0 , (struct sockaddr *)&server, &server_len) < 0)
		{
			fprintf(stderr, "recvfrom error\n");
			exit(1);
		}
		if(rbuf[0] == 'c') // If file has beed completed
			fileComplete[f[0]] = 1;
		if(rbuf[0] == 'f')	// If file was out of order
		{
			lineN[f[0]] = f[1];
			charN[f[0]] = f[2];
			seqNum[f[0]]--;
		}

		TCF=0;
		for(int n=0; n<MAXFILE; ++n)
			TCF = TCF + fileComplete[n]; // Counts the number of completed files

	}
	printf("All data sent.\n\n");
	char kill[5] = "kill\0";
	// Once all of the files are sent, send termination
	if(sendto(sd, kill, data_size, 0, (struct sockaddr *)&server, server_len) == -1)
		{
			fprintf(stderr, "sendto error\n");
			exit(1);
		}
	
	seq=0;
	int count=0;
	memset(nbuf, '\0', sizeof(nbuf));
	FILE *fp;
	printf("Receiving new data...\n");
	while(1) // While the cat file is not complete
	{
		memset(rbuf, '\0', sizeof(rbuf));
		
		if (recvfrom(sd, rbuf, MAXLEN, 0 , (struct sockaddr *)&server, &server_len) < 0)
			{
				fprintf(stderr, "recvfrom error\n");
				exit(1);
			}
		
		if(!strcmp(rbuf, "kill")) // Checks if end of transmission
			break;
		getData(rbuf, &fileNum, &sqNum, &txtSize, &fileSize, name); // Breaks apart payload
		if(seq == sqNum && (fileTotal + txtSize) < MAXLEN) // Adds payload to buffer until it exceeds MAXLEN
		{
			if(seq == 0)
				strcpy(nbuf, remHead(rbuf, txtSize));
			else
				strcat(nbuf, remHead(rbuf, txtSize));
			memset(rbuf, '\0', sizeof(rbuf));
			complete[0] = 'n';
			newFileTot += txtSize;
			fileTotal += txtSize;
			seq++;
		} else if((fileTotal + txtSize) >= MAXLEN && !count) // If MAXLEN is exceeded for the first time, save buffer to file
		{
			fp = fopen("./CatFile.txt", "w");
			if (fp == NULL) 
	    		{ 
				fprintf(stderr,"Could not open file ./CatFile.txt\n"); 
				exit(1); 
	    		} 			
			fprintf(fp, "%s", nbuf);
			fclose(fp);
			memset(nbuf, '\0', sizeof(nbuf));
			memset(rbuf, '\0', sizeof(rbuf));
			complete[0] = 'f';	
			fileTotal = 0;
			count++;
		} else if((fileTotal + txtSize) >= MAXLEN && count) // If MAXLEN is exceeded again, append buffer to file
		{
			fp = fopen("./CatFile.txt", "a");
			if (fp == NULL) 
	    		{ 
				fprintf(stderr,"Could not open file ./CatFile.txt\n"); 
				exit(1); 
	    		} 
			fprintf(fp, "%s", nbuf);
			fclose(fp);
			memset(nbuf, '\0', sizeof(nbuf));
			memset(rbuf, '\0', sizeof(rbuf));
			complete[0] = 'f';	
			fileTotal = 0;
		}else // If incorrect payload
		{
			memset(rbuf, '\0', sizeof(rbuf));
			complete[0] = 'f';		
		}
		// Sends response payload
		if(sendto(sd, complete, 1, 0, (struct sockaddr *)&server, server_len) == -1)
		{
			fprintf(stderr, "sendto error\n");
			exit(1);
		}
		if(newFileTot == fileSize)
			break;
		nbuf[strlen(nbuf)] = '\0';
		
	}

	gettimeofday(&end, NULL); // end delay measure

	if(count) // If file had already be written to, then apend
	{
		fp = fopen("./CatFile.txt", "a");
			if (fp == NULL) 
	    		{ 
				fprintf(stderr,"Could not open file ./CatFile.txt\n"); 
				exit(1); 
	    		} 
	} else // If file has not been written to
	{
		fp = fopen("./CatFile.txt", "w");
			if (fp == NULL) 
	    		{ 
				fprintf(stderr,"Could not open file ./CatFile.txt\n"); 
				exit(1); 
	    		} 
	}
	fprintf(fp, "%s", nbuf); // Saves buffer to file
	fclose(fp);
	close(sd);
	return delay(start, end);
}


char *listdir( const char *dir, int *fc, int fileNumber)
{
	// Looks in file directory and returns random file name	
	DIR *d = opendir(dir);  if( d == NULL) { perror(dir); return "DNE"; }
	struct dirent *dent; 
	int fileCount = 0;
	char *f = malloc(256*sizeof(char));	
	
	while (( dent = readdir(d)) != NULL)
	{
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            		continue;
		else
		{
			if(fileCount ==  fileNumber)
			{
				memset(f, '\0', 256);	
				strcpy(&f[0], dent->d_name);	
				f[strlen(dent->d_name)] = '\0';
			}
			fileCount++;
		}		
	}
	*fc = fileCount; // Number of files
	closedir(d);
	return(f);
}

char *getLine(char *folder, int fileComplete[MAXFILE], int lineN[MAXFILE], int seq[MAXFILE], int charN[MAXFILE], int f[3])
{

	char *fileLocation = malloc(MAXLEN*sizeof(char));
	char temp[sizeof(folder)];
	strcpy(temp, folder);
	int NOF;
	char *ch;
	ch = malloc(MAXLEN*sizeof(char));

	char *txt = malloc(MAXLEN*sizeof(char));
	// get file location
	
	int randFileNumber;
	do{
		randFileNumber = rand() % MAXFILE; // Get random file number that has not been completed
	
	}while(fileComplete[randFileNumber]);
	seq[randFileNumber]++; // Increments file sequence number
	// Small storage array
	f[0] = randFileNumber;
	f[1] = lineN[randFileNumber];
	f[2] = charN[randFileNumber];
	int randLine = (rand() % 3) + 1; // Gets random number of lines

	// Creates file location string
	strcpy(fileLocation, temp);
	strcpy(temp, listdir(folder, &NOF, randFileNumber));
	strcat(fileLocation, temp);

	// open file
	FILE * fp;
	fp = fopen(fileLocation, "r"); // Opens file
	// Check if file exists 
    	if (fp == NULL) 
    	{ 
        	fprintf(stderr,"Could not open file %s\n", fileLocation); 
        	exit(1); 
    	} 

	size_t pos = ftell(fp);    						// Gets the current position
	fseek(fp, 0, SEEK_END);    						// Goes to the end of the file
	size_t length = ftell(fp);						// Gets the position of the end which is the size
	fseek(fp, pos, SEEK_SET);  						// Goes to the original position of the file
	long int size=length;							// Gets file size
	
	if(size>MAXLEN)								// Exits if the file exceeds 2048 Bytes
	{
		fprintf(stderr, "\n\nYour file exceeds 2048 Bytes!\n");
		fprintf(stderr, "\nSize of file: %ld Bytes\n", size);
		fclose(fp);
		exit(1);
	}
	// Get number of lines in file
	char c;
	int lineCount = 0, charCount = 0;
	int i=0;
	int maxline = lineN[randFileNumber] + randLine; // Max line number 
	c = getc(fp);
	while((lineCount < lineN[randFileNumber]) && c != EOF) // If lines have already been sent
	{
		
	
		if(c == '\n') // Increment count if this character is newline 
            		lineCount++;
		c = getc(fp);
	}
	while((lineCount < maxline) && c != EOF) // Lines that need to be sent
	{
		
		txt[i++] = c;
		charCount++;
		if(c == '\n') // Increment count if this character is newline 
            		lineCount++;
		c = getc(fp);
	}
	
	charN[randFileNumber] += charCount;
	lineN[randFileNumber] += randLine;

	txt[i] = '\0';
	memset(ch, '\0', MAXLEN*sizeof(char));

	ch = addHeader(txt, randFileNumber, seq[randFileNumber], charCount, size, temp); // Adds header to payload
	fclose(fp);
	return ch;

}

long delay (struct timeval t1, struct timeval t2)
{
	long d;
	d = (t2.tv_sec - t1.tv_sec) * 1000;
	d += ((t2.tv_usec - t1.tv_usec + 500) / 1000);
	return(d);
}
