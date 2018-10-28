/*****************************************************************************
 * Ricky Haynes							    	     *
 * remHead.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program removes the header and trailers from the payload	     *
 *									     *
 *****************************************************************************/
#include "UDP.h"

char *remHead(char *ch, int txtSize)
{
	char nbuf[MAXLEN];
	strcpy(nbuf, ch);
	memcpy(&ch[0], nbuf+HEAD, txtSize); // Removes the header
	ch[txtSize] = '\0'; // Removes the etx byte
	return ch;
}
