#______________________________________LOADERMAKEFILE________________________________

CC = gcc
CFILE = loader.c
CFLAGS = -Wall -Werror -Wextra -fPIC
LDFLAGS = -shared
#Create lib_simpleloader.so from loader.c
all: lib_simpleloader.so

lib_simpleloader.so: $(CFILE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o lib_simpleloader.so $(CFILE)

#Provide code for clean up
clean:
	rm -f lib_simpleloader.so 

_________________________________LAUNCHERMAKEFILE___________________________________
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC
LDFLAGS = -L./bin -lsimpleloader #for creating  a link for simple loader
BINARY = bin
#Compile the launch.c by linking it with the lib_simpleloader.so
all: $(BINARY)/launch
$(BINARY)/launch: launch.c $(BINARY)/libsimpleloader.so
	$(CC) $(CFLAGS) -o $(BINARY)/lib_simpleloader.so launcher.c

#Provide code for clean up
clean:
	rm -f $(BINARY)/lib_simpleloader.so $(BINARY)/launch

______________________________________MAINMAKEFILE_________________________________
CC = gcc
CFLAGS = -Werror -Wall -Wextra -fPIC
LDFLAGS = -L./bin -l_simpleloader
BINARY = bin
#invoke make inside the following direcotries in the following order: loader launcher fib
 all: loader launcher fib
loader:
	@%(MAKE) -C loader
launcher:
	@$(MAKE) -C launcher
fib:
	@$(MAKE) -C fib
#move the lib_simpleloader.so and launch binaries inside bin direcotries
$(BINARY)/launch: launcher/launch.c $(BINARY)/lib_simpleloader.so
	$(CC) $(CFLAGS) $(LDFLAGS) -0 $(BINARY)/launch launcher/launch.c -L$(BINARY) -lsimpleloader
$(BINARY)/lib_simpleloader.so : loader/loader.C
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARY)/lib_simpleloader.so loader/loader.c
#provide the command for clean up
clean:
	@$(MAKE) -C loader clean
	@$(MAKE) -C launcher clean
	@$(MAKE) -C fib clean
____________________________________________________________________________________