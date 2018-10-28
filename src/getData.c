/*****************************************************************************
 * Ricky Haynes							    	     *
 * getData.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program retrieves data from the header of the payload		     *
 *									     *
 *****************************************************************************/
#include "UDP.h"

void getData(char b[MAXLEN], int *fileNum, int *seq, int *txtSize, int *fileSize, char *name)
{
	int i = 1, j = 0;
	*fileNum = ((b[i++]-'0')*100) + ((b[i++]-'0')*10) + (b[i++]-'0'); // Gets file number
	*seq = ((b[i++]-'0')*100) + ((b[i++]-'0')*10) + (b[i++]-'0');	// Gets sequence number
	*txtSize = ((b[i++]-'0')*1000) + ((b[i++]-'0')*100) + ((b[i++]-'0')*10) + (b[i++]-'0'); // Gets length of text string
	*fileSize = ((b[i++]-'0')*10000) +((b[i++]-'0')*1000) + ((b[i++]-'0')*100) + ((b[i++]-'0')*10) + (b[i++]-'0'); // Gets length of file
	i++;
	while(1)
	{
		if(b[i] != '|')		
			name[j++] = b[i++]; // Gets file name
		else
			break;
	}
}
