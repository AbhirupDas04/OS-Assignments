CC = gcc
CFLAGS = -m32  
BINARY = bin

#Compile the launch.c by linking it with the lib_simpleloader.so

$(BINARY)/launch: launch.c
	$(CC) -I../loader $(CFLAGS) -o launch ../loader/lib_simpleloader.so ../loader/loader.c launch.c

#Provide code for clean up

clean:
	rm -f lib_simpleloader.so launch