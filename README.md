#   OS ASSIGNMENT-1 (with-bonus)
```
                                                                                    Abhirup Das(2022019) | Armaan Singh(2022096)
```                             
## SimpleLoader: Creating an ELF Loader in C from Scratch
```
This private repository contains the code for a simple Loader for a  ELF 32-bit executable file in C from scratch without the use of any library APIs available for manipulating ELF files. It can compile to a shared library(lib_simpleloader.so) that can be used to load and execute the executable.
```
## Structure of the files in the Repository
```
OS-ASSIGNMENTS
    |-with-bonus
    |    |---bin
    |    |
    |    |---launcher
    |    |      |---launch.c
    |    |      |---Makefile
    |    |
    |    |---loader
    |    |      |---loader.c
    |    |      |---loader.h
    |    |      |---Makefile
    |    |
    |    |---test
    |    |      |---fib.c
    |    |      |---Makefile
    |    |
    |    |---Makefile
    |
    |---without-bonus
            |---fib.c
            |---loader.c
            |---loader.h
            |---Makefile

```
## launcher
### launch.c
```
This program is designed to load and execute ELF 32-bit executables using a custom ELF loader implemented in C.
+-------------------+
|#include "loader.h"|
+-------------------+
This line includes the header file "loader.h," which presumably contains the necessary declarations and prototypes for functions used in the program.
+-----------------------------------------------------+
|int main(int argc, char** argv)                      |
|{                                                    |
|  if(argc != 2) {                                    |
|    printf("Usage: %s <ELF Executable> \n",argv[0]); |
|    exit(1);                                         |
|  }                                                  |   
+-----------------------------------------------------+
The main function is the entry point of the program. It takes command-line arguments argc (argument count) and argv (argument vector) as parameters. The code checks if the number of command-line arguments is not equal to 2 (indicating an incorrect number of arguments). If so, it prints a usage message and exits the program with an exit status of 1.
+------------------------------------------------------------------------+
|  // 1. carry out necessary checks on the input ELF file                |
|  // 2. passing it to the loader for carrying out the loading/execution |
|  load_and_run_elf(argv[1]);                                            |
|  // 3. invoke the cleanup routine inside the loader                    |
|  loader_cleanup();                                                     |
|  return 0;                                                             |
|}                                                                       |                           
+------------------------------------------------------------------------+
 The function load_and_run_elf handles the ELF loading and execution process, and the loader_cleanup() performs the clean up such as releasing allocated memory and resources related to the ELF loading process.
 ```
### Makefile
```
 This makefile is used to compile the launch.c program by linking it with the lib_simpleloader.so library. It also includes a target for cleaning up the generated executable. 
+--------------+
|CC = gcc      |
|CFLAGS = -m32 | 
|BINARY = bin  |
+--------------+
-CC specifies the compiler to be used
-CFLAGS specifies the compilation flags to be used (-m32 specifies that the program should t=be compiled for a 32 bit target)
-BINARY specifies the name of the output directory (bin) where the compiled executables would be placed
+----------------------------------------------------------------------------------------------------+
|#Compile the launch.c by linking it with the lib_simpleloader.so                                    |
|                                                                                                    |
|$(BINARY)/launch: launch.c                                                                          |
|	$(CC) -I../loader $(CFLAGS) -o launch ../loader/lib_simpleloader.so ../loader/loader.c launch.c  |
+----------------------------------------------------------------------------------------------------+
This part of the Makefile defines a target named $(BINARY)/launch, which is the output executable file launch placed in the bin directory. The rule specifies how to compile launch.c by linking it with the lib_simpleloader.so library and the loader.c file from the ../loader directory.
+---------------------------+
|#Provide code for clean up |
|                           |
|clean:                     |
|	rm -f launch            |
+---------------------------+
When you run the make clean command, it will remove the launch executable file using the rm -f command. This helps to clean up the build artifacts and leave the directory tidy.
```
## loader
### loader.c
```
This program loads and executes an ELF executable file and performs necessary cleanup after execution.
+---------------------------------------------------------------------------+
|Elf32_Ehdr *ehdr;                                                          |
|Elf32_Phdr *phdr;                                                          |
|int fd;                                                                    |
|                                                                           |   
|/*                                                                         |
| * release memory and other cleanups                                       |
| */                                                                        |
|void loader_cleanup() {                                                    |
|  free(ehdr);  // Free memory allocated for the ELF header.                |
|  free(phdr);  // Free memory allocated for the program headers.           |
|                                                                           |
|  // Close the file descriptor used for opening the ELF executable file.   |
|  if(close(fd) == -1){                                                     |
|    perror("ERROR");                                                       |
|    exit(1);                                                               |
|  };                                                                       |
|}                                                                          |
+---------------------------------------------------------------------------+
The 'loader_cleanup' is responsible for releasing memory and performing cleanup tasks. It frees the memory allocated for the ELF header and program headers. It also closes the file descriptor that was used to open the ELF executable file.
+--------------------------------------------------------------------------------------------------------------------------------+
|/*                                                                                                                              |
| * Load and run the ELF executable file                                                                                         |
| */                                                                                                                             |
|void load_and_run_elf(char** exe) {                                                                                             |
|  if((fd = open(*exe, O_RDONLY))==-1){                                                                                          |
|    perror("ERROR");                                                                                                            |
|    exit(1);                                                                                                                    |
|  };                                                                                                                            |
|                                                                                                                                |
|  // 1. Load entire binary content into the memory from the ELF file.                                                           |
|                                                                                                                                |
|  // Allocate memory for the ELF header and read it from the file.                                                              |
|  ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));                                                                               |
|  ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr));                                                                |
|  if(NBytes_Read_EHDR == -1){                                                                                                   |
|    perror("ERROR");                                                                                                            |
|    exit(1);                                                                                                                    |
|  }                                                                                                                             |
|                                                                                                                                |
|  // Allocate memory for the program headers and read them from the file.                                                       |
|  phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum));                                                      |
|  ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum));                                       |
|  if(NBytes_Read_PHDR == -1){                                                                                                   |
|    perror("ERROR");                                                                                                            |
|    exit(1);                                                                                                                    |
|  }                                                                                                                             |
|   // 2. Iterate through the PHDR table and find the section of PT_LOAD                                                         |
|  //    type that contains the address of the entrypoint method in fib.c                                                        |
|                                                                                                                                |
|  Elf32_Phdr Req_Prog_Header;                                                                                                   |
| int index;                                                                                                                     |
|  int flag = 0;                                                                                                                 |
|                                                                                                                                |
|  // Iterate through program headers to find the one that contains the entry point.                                             |
|  for(int i = 0; i < ehdr->e_phnum; i++){                                                                                       |
|    if(phdr[i].p_type == PT_LOAD && phdr[i].p_vaddr < ehdr->e_entry && phdr[i].p_vaddr + phdr[i].p_memsz > ehdr->e_entry){      |
|        Req_Prog_Header = phdr[i];  // Store the program header that contains the entry point.                                  |
|        index = i;                                                                                                              |
|        flag = 1;                                                                                                               |
|        break;                                                                                                                  |
|    }                                                                                                                           |
|  }                                                                                                                             |
|                                                                                                                                |
|  if(flag == 0){                                                                                                                |
|    printf("PT_LOAD Not Found.\n");                                                                                             |
|    exit(1);                                                                                                                    |
|  }                                                                                                                             |
|                                                                                                                                |
|  // 3. Allocate memory of the size "p_memsz" using mmap function                                                               |
|  //    and then copy the segment content                                                                                       |
|                                                                                                                                |
|  // Allocate memory for the segment using mmap.                                                                                |
|  void* virtual_mem = mmap(NULL, Req_Prog_Header.p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0,0);|
|  if(virtual_mem == (void*)(-1)){                                                                                               |
|    printf("MMAP ERROR\n");                                                                                                     |
|    exit(1);                                                                                                                    |
|  }                                                                                                                             |
|                                                                                                                                |
|  // Move the file cursor to the offset of the segment content.                                                                 |
|  if(lseek(fd,phdr[index].p_offset,SEEK_SET) == -1){                                                                            |
|    perror("ERROR");                                                                                                            |
|    exit(1);                                                                                                                    |
|  };                                                                                                                            |
|                                                                                                                                |
|  // Read the segment content from the file into the allocated memory.                                                          |
|  if(read(fd, virtual_mem,phdr[index].p_memsz) == -1){                                                                          |
|    perror("ERROR");                                                                                                            |
|    exit(1);                                                                                                                    |
|  };                                                                                                                            |
|                                                                                                                                |
|                                                                                                                                |
+--------------------------------------------------------------------------------------------------------------------------------+
 The function begins by opening the specified ELF executable file using the open system call, checking for errors in the process. Subsequently, it reads the ELF header (EHDR) and program headers (PHDR) from the file into memory. The EHDR contains essential information about the structure of the ELF file, and the PHDRs describe the various segments and sections within the file.
After loading the headers, the function iterates through the program headers to identify the segment that contains the entry point of the program. It does this by checking for segments with type PT_LOAD whose memory range includes the entry point address. If a suitable segment is found, it is stored along with its index for later use. If no such segment is found, an error is displayed, and the function exits.
The function then allocates memory using the mmap function to create a memory region for the loaded segment. The memory region is allocated with read, write, and execute permissions to enable code execution. The segment's content is read from the file and copied into the allocated memory region using the lseek and read system calls.
Next, the function calculates the address where the entry point of the loaded program resides within the allocated memory region. This address is used to typecast a function pointer to match the _start method's signature within the loaded program. The _start method typically serves as the program's entry point and initialization routine.
The _start method is then invoked using the function pointer, and its return value is captured. Finally, the function prints the value returned by _start, indicating the success or result of the program's execution. After execution is complete, the allocated memory region is released using the munmap function.
In summary, the load_and_run_elf function encapsulates the process of loading an ELF executable, setting up memory, executing the program, and performing necessary cleanup tasks. It plays a critical role in the dynamic loading and execution of ELF binaries using the SimpleLoader approach.
```
### loader.h
```
+----------------------------------------+
|/*                                      |
| * No changes are allowed to this file  |
| */                                     |
|                                        |
|#include <stdio.h>                      |
|#include <elf.h>                        |
|#include <string.h>                     |
|#include <fcntl.h>                      |
|#include <stdlib.h>                     |
|#include <unistd.h>                     |
|#include <assert.h>                     |
|#include <sys/types.h>                  |
|#include <sys/mman.h>                   |
|                                        |
|void load_and_run_elf(char** exe);      |
|void loader_cleanup();                  |
+----------------------------------------+
It contains all the neccessary imports for loader.c to work
```
### Makefile
```
+------------------+
|CC = gcc          |
|CFILE = loader.c  |
|CFLAGS = -m32     |
|LDFLAGS = -shared |
+------------------+
-CC specifies the compiler to be used
-CFLAGS specifies the compilation flags to be used (-m32 specifies that the program should t=be compiled for a 32 bit target)
-LDFLAGS is assigned -shared indicating that the output should be a shared library
-CFILE specifies the source file that needs to be compiled
+---------------------------------------------------------------+
|#Create lib_simpleloader.so                                    |
|                                                               |
|lib_simpleloader.so: $(CFILE)                                  |
|	$(CC)   $(CFLAGS) $(LDFLAGS) -o lib_simpleloader.so $(CFILE)|
+---------------------------------------------------------------+
This part defines a target (lib_simpleloader.so) that depends on the source file loader.c. It specifies that the target file should be built if the source file changes.
+------------------------------+
|#Provide code for clean up    |
|                              |
|clean:                        |
|	rm -f lib_simpleloader.so  |
+------------------------------+
It removes the lib_simpleloader.so file, essentially cleaning up the project.
```
## test
### fib.c
```
+-----------------------------------------+
|/*                                       |
| * No changes are allowed in this file   |
| */                                      |
|int fib(int n) {                         |
|  if(n<2) return n;                      |
|  else return fib(n-1)+fib(n-2);         |
|}                                        |
|                                         |
|int _start() {                           |
|	int val = fib(40);                    |
|	return val;                           |
|}                                        |
+-----------------------------------------+
This file is a basic implementation of fibonacci series in c.
Here we are passing 40 into the code.
```
### Makefile
```
+------------------------------------------------------------------------------------+
|#Create 32-bit executable for fib.c by using the gcc flags as mentioned in the PDF  |
|all:                                                                                |
|	gcc -m32 -no-pie -nostdlib -o fib fib.c                                          |
|#Provide the command for cleanup                                                    |
|clean:                                                                              |
|	-@rm -f fib                                                                      |
+------------------------------------------------------------------------------------+
This makefile is used to create a 32 bit executable for fib.c and also provide a clean up for it.
```
## Makefile
```
+--------------------------------+
|.PHONY: all loader launcher test|
+--------------------------------+
The .PHONY targets are special targets that are not associated with actual files. They tell Make that these targets don't correspond to files and will always be executed when invoked.
+-------------------------+
|loader:                  |
|	@$(MAKE) -C loader    |
|launcher:                |
|	@$(MAKE) -C launcher  |
|test:                    |
|	@$(MAKE) -C test      |
+-------------------------+
These targets use $(MAKE) to invoke Make in the respective subdirectories (loader, launcher, and test) to build their contents
@ - is used to supress the echoing commands
+-----------------------------------------------+
|$(BINARY)/launch: launcher/launch.c            |
|	mv launcher/launch bin                      |
|$(BINARY)/lib_simpleloader.so: loader/loader.c |
|	mv loader/lib_simpleloader.so bin           |
+-----------------------------------------------+
They move the compiled launch binary from the launcher directory to the bin directory and the compiled lib_simpleloader.so from the loader directory to the bin directory.
+---------------------------------+
|clean:                           |
|	@$(MAKE) -C loader clean      |
|	@$(MAKE) -C launcher clean    |
|	@$(MAKE) -C test clean        |
|	rm -f bin/launch              |
|	rm -f bin/lib_simpleloader.so |
+---------------------------------+
This invokes the clean targets in the subdirectories (loader, launcher, test) to clean their builds. It also removes the launch and lib_simpleloader.so files from the bin directory.
```




        