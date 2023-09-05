CC=gcc
CFLAGS=-I.
DEPS = Shellcommands/shell.h
OBJ = Shellcommands/shell.o
BIN = bin/shell

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	mv $(BIN) bin/

.PHONY: clean

clean:
	rm -f $(OBJ) $(BIN)