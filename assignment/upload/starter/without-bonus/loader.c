#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  free(ehdr);
  free(phdr);
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  fd = open(*exe, O_RDONLY);
  // 1. Load entire binary content into the memory from the ELF file.

  ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
  ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr));
  if(NBytes_Read_EHDR == -1){
    printf("Error in Reading EHDR");
    exit(1);
  }

  phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum));
  ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum));
  if(NBytes_Read_PHDR == -1){
    printf("Error in Reading PHDR");
    exit(1);
  }

  // 2. Iterate through the PHDR table and find the section of PT_LOAD 
  //    type that contains the address of the entrypoint method in fib.c

  Elf32_Phdr Req_Prog_Header;
  int index;

  for(int i = 0; i < ehdr->e_phnum; i++){
    if(phdr[i].p_type == PT_LOAD && phdr[i].p_vaddr < ehdr->e_entry && phdr[i].p_vaddr + phdr[i].p_memsz > ehdr->e_entry){
        Req_Prog_Header = phdr[i];
        index = i;
        break;
    }
  }

  // 3. Allocate memory of the size "p_memsz" using mmap function 
  //    and then copy the segment content

  void* virtual_mem = mmap(NULL, Req_Prog_Header.p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0,0);
  if(virtual_mem == (void*)(-1)){
    printf("MMAP ERROR");
    exit(1);
  }

  lseek(fd,phdr[index].p_offset,SEEK_SET);
  read(fd, virtual_mem,phdr[index].p_memsz);

  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step

  void* address =  virtual_mem + (ehdr->e_entry - phdr[index].p_vaddr);

  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.

  int (*_start)() = (int(*)())(address);

  // 6. Call the "_start" method and print the value returned from the "_start"

  int result = _start();
  printf("User _start return value = %d\n",result);

  munmap(virtual_mem,Req_Prog_Header.p_memsz);
}

int main(int argc, char** argv) 
{
  if(argc != 2) {
    printf("Usage: %s <ELF Executable> \n",argv[0]);
    exit(1);
  }
  // 1. carry out necessary checks on the input ELF file
  // 2. passing it to the loader for carrying out the loading/execution
  load_and_run_elf(&argv[1]);
  // 3. invoke the cleanup routine inside the loader  
  loader_cleanup();
  return 0;
}
