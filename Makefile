# Headers
HEAD = UDP.h
# The compiler to use
CC = gcc

# Options flags to pass to the compiler
CFLAG1 = -c 
CFLAG2 = -fPIC
CFLAG3 = -Wall
CFLAG4 = -std=c11
# Src and Build (i.e. obj) directories
VPATH = src
BUILDDIR = obj

# Compile each .c src file to a .o file without linking 
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAG4) $(CFLAG1) $< -o $@ 

# Link all .o files with gcc to get the final executable
./udp: $(BUILDDIR)/UDPmain.o $(BUILDDIR)/UDPclient.o $(BUILDDIR)/UDPserver.o $(BUILDDIR)/addHeader.o $(BUILDDIR)/getData.o $(BUILDDIR)/remHead.o  
	$(CC) $(BUILDDIR)/UDPmain.o $(BUILDDIR)/UDPclient.o $(BUILDDIR)/UDPserver.o $(BUILDDIR)/addHeader.o $(BUILDDIR)/getData.o $(BUILDDIR)/remHead.o -o ./udp

clean:
	rm -rf $(BUILDDIR)/*o ./udp



