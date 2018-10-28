/*****************************************************************************
 * Ricky Haynes							    	     *
 * UDP.h								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This is a header file for project 1 					     *
 *									     *
 *****************************************************************************/
#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define SERVER_UDP_PORT 7777
#define MAXLEN 16384
#define DEFLEN 64
#define HEAD 40 
#define MAXFILE 10
#define IP_LEN 15

// Layout of header
// /000/000/000/00000/000...000/ 
// file#/seq/chcount/filesize/filename/

long int UDPclient(char *host, char *folder);
int UDPserver();
char *addHeader(char *buf, int fileNum, int seqNum, int charCnt, int tFileSize, char *name);
char *remHead(char *ch, int txtSize);
void getData(char b[MAXLEN], int *fileNum, int *seq, int *txtSize, int *fileSize, char *name);

#endif
