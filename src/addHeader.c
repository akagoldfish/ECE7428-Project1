/*****************************************************************************
 * Ricky Haynes							    	     *
 * addHeader.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program adds headers and trailers to payload			     *
 *									     *
 *****************************************************************************/
#include "UDP.h"
// Layout of header
// /000/000/0000/00000/000...000/ 
// file#/seq/chcount/filesize/filename/

char *addHeader(char *buf, int fileNum, int seqNum, int charCnt, int tFileSize, char *name)
{
	char *ch = malloc(MAXLEN*sizeof(char));
	char tArray[3];	
	char sArray[3];	
	char cArray[4];
	char fArray[5];
	long int size = sizeof(ch); 
	
	memset(ch, '\0', size);
	long int count = fileNum;
	// create array for file number
	tArray[2] = (count % 10)+'0';
	count = count - (count % 10);
	tArray[1] = ((count % 100)/10)+'0';
	count = count - (count % 100);
	tArray[0] = (count / 100)+'0';

	// create array for seqence of each file
	count = seqNum;
	sArray[2] = (count % 10)+'0';
	count = count - (count % 10);
	sArray[1] = ((count % 100)/10)+'0';
	count = count - (count % 100);
	sArray[0] = (count / 100)+'0';

	// create array for # of characters in txt 
	count = charCnt;
	cArray[3] = (count % 10)+'0';
	count = count - (count % 10);
	cArray[2] = ((count % 100)/10)+'0';
	count = count - (count % 100);
	cArray[1] = ((count % 1000) / 100)+'0';
	count = count - (count % 1000);
	cArray[0] = (count / 1000)+'0';

	// create array for total size of file 
	count = tFileSize;
	fArray[4] = (count % 10)+'0';
	count = count - (count % 10);
	fArray[3] = ((count % 100)/10)+'0';
	count = count - (count % 100);
	fArray[2] = ((count % 1000) / 100)+'0';
	count = count - (count % 1000);
	fArray[1] = ((count % 10000) / 1000)+'0';
	count = count - (count % 10000);
	fArray[0] = (count / 10000)+'0';

	int fileNameLen = strlen(name);
	int i = 0;
	ch[i] = 0x02; // Adds stx char to payload
	i++;
	memcpy(&ch[i], tArray, sizeof(tArray)); // Adds file number to payload
	i+=3;
	memcpy(&ch[i], sArray, sizeof(sArray)); // Adds sequence number to payload
	i+=3;
	memcpy(&ch[i], cArray, sizeof(cArray)); // Adds char count to payload
	i+=4;
	memcpy(&ch[i], fArray, sizeof(fArray)); // Adds file size to payload
	i+=5;
	ch[i] = '|'; // Adds separating char to payload
	i++;
	memcpy(&ch[i], name, fileNameLen); // Adds file name to payload
	ch[fileNameLen + i] = '|'; // Adds separating char to payload
	i++;
	memset(&ch[fileNameLen + i], '0', HEAD - fileNameLen + i); // Fills the rest of the header with 0's
	strcpy(&ch[HEAD], buf); // Adds the text to payload
	ch[charCnt+HEAD] = 0x03; // Adds etx char to payload
	ch[charCnt+HEAD+1] = '\0'; // Adds NULL byte to the end of the buffer
	//printf("ch = %s\n", ch);

	return ch;
}
