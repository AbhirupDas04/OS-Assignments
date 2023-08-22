CC = gcc
CFILE = loader.c
CFLAGS = -m32 
LDFLAGS = -shared

#Create lib_simploader.so

lib_simpleloader.so: $(CFILE)
	$(CC)   $(CFLAGS) $(LDFLAGS) -o lib_simpleloader.so $(CFILE)

#Provide code for clean up

clean:
	rm -f lib_simpleloader.so