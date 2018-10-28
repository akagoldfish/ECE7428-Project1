# ECE 7428 (Computer Comm. Networks) Project 1
## Author: Richard Haynes
### About
1. A server process listens on UDP port 7777, waiting for a client to send it UDP data segments.
2. Next, a client process starts execution and gets the IP address of the remote server, which you can supply manually from the terminal window, or from a stored file. The client process may run on the same machine as the server, or on a different machine on a different network.
3. The client then proceeds to transfer ten files to the server. The server does not have any a priori information about the client’s transmission schedule. Therefore, a header is added to inform the server of the filename, the total number of lines in that file, and the line number associated with each transmitted line of the file. 
4. Once the client finishes all transmissions, the server concatenates all files into a single large file and sends it back to the client, with all files and (lines within those files) in order. 

### Usage
#### File generator
To generate ten random files, genFiles.sh can be used. 
Its usage is `./genFiles.sh [-l] QUANTITY DIRECTORY`, where `QUANTITY` is how many files to generate, `DIRECTORY` is where those files should be created, and `-l` is used to create larger text files.  
For example, to create ten files in the directory ‘files’, `./genFiles.sh 10 ./files/` can be entered. 
#### File transmission
To compile and run the actual program, I created a Makefile to compile my program for me using gcc from the terminal. Once in the correct directory, running the command make will automatically compile and build the program. To run the server just use the following: ./udp s. Once the server is running, then the client can transmit the files. To run the client use the following: 
`./udp c [-h server_file] server file_folder` 
Enter the IP address of the server for `server`, or use `-h server_file` instead if you have a file with just the ip address, where `server_file` is the location of that file. Then enter the location of the files you wish to send for `file_folder`. The files should then be sent to the server. Once the server receives all of the files it will concatenate all of the files into a single large file and send it back to the client. The client will save the new concatenated file into its current directory as `CatFile.txt`.

