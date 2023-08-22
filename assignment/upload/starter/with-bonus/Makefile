CC = gcc
CFLAGS = -m32 
BINARY = bin

#invoke make inside the following directories in the following order: loader launcher test

.PHONY: all loader launcher test #.PHONY is to avoid conflict 
all:	loader	launcher	test	$(BINARY)/launch	$(BINARY)/lib_simpleloader.so
loader:
	@$(MAKE) -C loader
launcher:
	@$(MAKE) -C launcher
test:
	@$(MAKE) -C test

#move the lib_simpleloader.so and launch binaries inside bin directories

$(BINARY)/launch:	launcher/launch.c	$(BINARY)/lib_simpleloader.so
	mv launcher/launch bin
$(BINARY)/lib_simpleloader.so:	loader/loader.c
	mv loader/lib_simpleloader.so bin

#provide the command for clean up

clean:
	@$(MAKE) -C loader clean
	@$(MAKE) -C launcher clean
	@$(MAKE) -C test clean
	rm -f bin/launch
	rm -f bin/lib_simpleloader.so