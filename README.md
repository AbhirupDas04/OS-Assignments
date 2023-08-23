#   OS ASSIGNMENT-1 
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
## with-bonus
### launcher.c
This program is designed to load and execute ELF 32-bit executables using a custom ELF loader implemented in C.
```
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




        