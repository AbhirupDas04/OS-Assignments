CC = gcc
CFLAGS = -m32 -Wall -Werror -Wextra -fPIC
LDFLAGS = -L./bin -lsimpleloader #for creating  a link for simple loader
BINARY = bin
#Compile the launch.c by linking it with the lib_simpleloader.so
$(BINARY)/launch: launch.c $(BINARY)/lsimpleloader.so
	$(CC) $(CFLAGS) -o $(BINARY)/lib_simpleloader.so launcher.c

#Provide code for clean up
clean:
	rm -f $(BINARY)/lib_simpleloader.so $(BINARY)/launch