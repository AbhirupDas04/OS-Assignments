#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  fd = open(argv[1], O_RDONLY);

  // 1. Load entire binary content into the memory from the ELF file.

  ehdr = (Elf32_Ehdr*)malloc((size_t)52);
  ssize_t NBytes_Read_EHDR = read(fd, ehdr, (size_t)52);

  phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum));
  ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum));

  // 2. Iterate through the PHDR table and find the section of PT_LOAD 
  //    type that contains the address of the entrypoint method in fib.c

  Elf32_Phdr* Req_Prog_Header;

  for(int i = 0; i < ehdr->e_phnum; i++){
    if(phdr->p_type == PT_LOAD && phdr->p_vaddr == ehdr->e_entry){
        Req_Prog_Header = phdr;
        break;
    }
    else{
        phdr += ehdr->e_phentsize;
    }
  }

  // 3. Allocate memory of the size "p_memsz" using mmap function 
  //    and then copy the segment content
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
  // 6. Call the "_start" method and print the value returned from the "_start"

  int result = _start();
  printf("User _start return value = %d\n",result);
}
