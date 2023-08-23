#   OS ASSIGNMENT-1 (with-bonus)
                             Abhirup Das(2022019) | Armaan Singh(2022096)
## SimpleLoader: Creating an ELF Loader in C from Scratch
This private repository contains the code for a simple Loader for a  ELF 32-bit executable file in C from scratch without the use of any library APIs available for manipulating ELF files. It can compile to a shared library(lib_simpleloader.so) that can be used to load and execute the executable.
## Structure of the files in the Repository
OS-ASSIGNMENTS
```bash
|-with-bonus
|    |---bin
|    |---launcher
|    |   |---launch.c
|    |   |---Makefile
|    |---loader
|    |    |---loader.c
|    |    |---loader.h
|    |    |---Makefile
|    |---test
|    |    |---fib.c
|    |    |---Makefile
|    |---Makefile
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
--CFLAGS specifies the compilation flags to be used (-m32 specifies that the program should t=be compiled for a 32 bit target)
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

```





        