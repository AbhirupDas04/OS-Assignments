# Assignment-4 - Simple Smart Loader
##### Abhirup Das | Armaan Singh



## File Structure
```
Assignment-4
|
|---bin
|
|---launcher
|     |---launch.c
|     |---Makefile
|
|---loader
|     |---loader.c
|     |---loader.h
|     |---Makefile
|
|---test
|     |---fib.c
|     |---sum.c
|     |---Makefile
|
|---Makefile
|---Readme.md
```

## Code Explanation
### loader
#### loader.c
```
- The loader.c file contains the implementation of a loader for ELF executable files
-The Escape_sequence function is a signal handler for handling segmentation faults (SIGSEGV). It is called when a segmentation fault occurs and performs the necessary actions to handle the fault.
-The loader_cleanup function is responsible for releasing memory and performing other cleanups.
- The load_and_run_elf function is the main function that loads and runs the ELF executable file.
- It opens the file, sets up the signal handler, and gets the page size.
- It then loads the ELF header and program header table into memory.
- It navigates to the entry point address and calls the _start method.
- Finally, it prints the return value of _start, the number of page faults, the number of page allocations, and the total amount of internal fragmentation.
```
#### loader.h
```
- The loader.h file contains the function declarations for the loader.
- It includes the necessary headers and declares the load_and_run_elf and loader_cleanup functions.
```
#### Makefile
```
- The Makefile is used to compile the loader.c file into a shared library named lib_simpleloader.so.
- It uses the gcc compiler with the -m32 flag to compile the code for 32-bit architecture.
```

### launcher
#### launch.c
```
- The launch.c file is used to load the loader and perform cleanup.
- It includes the loader.h header file.
- The main function takes command-line arguments and checks if only one additional parameter is passed to the launch executable.
- It opens the ELF executable file and reads its ELF header.
- It performs checks to ensure that the file is a valid ELF executable.
- After the checks, it closes the file and frees the allocated memory for the ELF header.
- It then calls the load_and_run_elf function from the loader to carry out the loading and execution of the ELF file.
- Finally, it invokes the loader_cleanup routine inside the loader to perform cleanup.
```
#### Makefile
```
The Makefile is used to compile the launch.c file by linking it with the lib_simpleloader.so library from the loader.
- It uses the gcc compiler with the -m32 flag to compile the code for 32-bit architecture.
```
### test
#### fib.c
```
- It is a regular fib.c file
- It is supposed to catch 1 page fault and 1 page allocation and an internal  fragmentation of  3.889 KB
```
#### sum.c
```
- It is just for calculating the sum of each element of an array(which is 2),size of array is 8192
- It is supposed to catch 10 page faults, 10 page allocations and an internal fragmentation of 7.889 KB
```
### Makefile
```
- The CC variable is set to gcc and CFLAGS is set to -m32, which specifies the compiler and compiler flags to use.
- The BINARY variable is set to bin, representing the directory where the binaries will be placed.
- The all target is the default target and it depends on the loader, launcher, and test targets, as well as the $(BINARY)/launch and $(BINARY)/lib_simpleloader.so targets.
- The loader, launcher, and test targets invoke make inside the respective directories to build the code in those directories.
- The $(BINARY)/launch target moves the launch binary from the launcher directory to the bin directory.
- The $(BINARY)/lib_simpleloader.so target moves the lib_simpleloader.so library from the loader directory to the bin directory.
- The clean target invokes make clean inside the loader, launcher, and test directories to clean up the build artifacts in those directories. It also removes the bin/launch and bin/lib_simpleloader.so files.
```
