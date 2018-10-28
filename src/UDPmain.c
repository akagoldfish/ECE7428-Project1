/*****************************************************************************
 * Ricky Haynes							    	     *
 * UDPmain.c								     *
 * Copyright (C) Richard Haynes 2018 <rhayne02@villanova.edu>		     *
 * 									     *
 * This program generates a single executable for an UDP client/server	     *
 * communication 							     *
 *									     *
 *****************************************************************************/
#include "UDP.h"

int main(int argc, char **argv)
{
	char *pname, *server, fserver[IP_LEN], *folder, mode;
	// Check command line arguments
	pname = argv[0];
	argc--;
	argv++;
	//server = malloc(IP_LEN*sizeof(char));

	if(argc < 1)
	{	
		fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
		exit(1);
	}	
	mode = *argv[0];
	argc--;
	argv++;
	switch (mode) {
		// If client mode
		case 'c':
		case 'C':
			// A file with the server is given
			
			if(argc > 0 && (strcmp(*argv, "-h") == 0)) 
			{
				FILE * serverFile;
				serverFile = fopen(*++argv, "r");
				if (serverFile == NULL) 
			    	{ 
					fprintf(stderr,"Could not open file %s\n", *argv); 
					exit(1); 
			    	} 
				if (--argc > 1 )
				{
					fscanf(serverFile, "%s", fserver);
					//argc--;
					argv++;
				}
				else
				{
					fprintf(stderr, "Unable to open file\n");
					fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
					exit(1);
				}
				
				if (argc > 0 ) // If folder name of the files is given
				{
					folder = *argv;
					if(strcmp(*argv+strlen(*argv)-1, "/"))
						strcat(folder, "/");
				}
				else 
				{
					fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
					exit(1);
				}
				
				server = fserver;
				server[strlen(server)] = '\0';
				fclose(serverFile);
				
			}
			else if( argc > 0) // If no server file is given
			{
				server = *argv;
				if (--argc > 0 ) // If folder name of the files is given
					folder = *++argv;
				else 
				{
					fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
					exit(1);
				}
			}
			else 
			{
				fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
				exit(1);
			}
			printf("All data sent & recieved in %ld ms.\n", UDPclient(server, folder)); // Runs main client program
			
			break;
		// If server mode
		case 's':
		case 'S': 
			UDPserver(); // Runs main server program
			printf("All data sent.\n");
			break;
		default :
			fprintf(stderr, "Usage: %s mode [-h server_file] server file_folder\nwhere, mode = 's' (Server) or 'c' (Client)\n", pname);
				exit(1);
		}
return 0;

}

