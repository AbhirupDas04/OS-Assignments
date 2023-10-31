#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;
int index2;
void* address;
int page_size;
Elf32_Phdr Req_Prog_Header;
int count = 0;
int curr_page_size;

void Escape_sequence(int signum, siginfo_t *info){
    if(signum == SIGSEGV){
      lseek(fd,0,SEEK_SET);
    //   printf("%p\n",info->si_addr);
      // if(munmap(virtual_mem,curr_page_size) == -1){
      //   printf("aunt dead\n");
      // }
      for(int i = 0; i < ehdr->e_phnum; i++){
        if(phdr[i].p_type == PT_LOAD && (void*)phdr[i].p_vaddr <= info->si_addr && (void*)phdr[i].p_vaddr + phdr[i].p_memsz >= info->si_addr){
          // printf("%p\n",phdr[i].p_vaddr);
            Req_Prog_Header = phdr[i];
            index2 = i;
            break; // break when we know that entrypoint lies in this segment
        }
      }

      void* start_page = info->si_addr - (int)info->si_addr % page_size;

      int offset = start_page - (void*)phdr[index2].p_vaddr;

      //void* virtual_mem =  mmap((void*)phdr[index2].p_vaddr, curr_page_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, phdr[index2].p_offset);
      void* virtual_mem =  mmap((void*)phdr[index2].p_vaddr + offset,page_size,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,-1,0);
      
      lseek(fd,phdr[index2].p_offset + offset,SEEK_SET);

      int n_bytes_read;
      if(phdr[index2].p_memsz - offset < page_size){
        n_bytes_read = phdr[index2].p_memsz - offset;
      }
      else{
        n_bytes_read = page_size;
      }
    //   printf("%d %d\n",n_bytes_read , phdr[index2].p_memsz);
      read(fd, virtual_mem,n_bytes_read);
      
      if(virtual_mem == NULL){
        printf("aunt\n");
      }
    }
}

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  free(ehdr);
  free(phdr);

  if(close(fd) == -1){
    perror("ERROR");
    exit(1);
  };
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  if((fd = open(*exe, O_RDONLY))==-1){
    perror("ERROR");
    exit(1);
  };

  struct sigaction sig_struct;
  memset(&sig_struct, 0, sizeof(sig_struct));
  sig_struct.sa_handler = (void (*)(int))Escape_sequence;
  sig_struct.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sig_struct, NULL);

  page_size = getpagesize();

  // 1. Load entire binary content into the memory from the ELF file.

  ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr)); // For allocating memory for ELF Header
  ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr));
  if(NBytes_Read_EHDR == -1){
    perror("ERROR");
    exit(1);
  }

  phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum)); // For allocating memory for Program Header Table
  ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum));
  if(NBytes_Read_PHDR == -1){
    perror("ERROR");
    exit(1);
  }

  // 2. Iterate through the PHDR table and find the section of PT_LOAD 
  //    type that contains the address of the entrypoint method in fib.c

  // int flag = 0;

  // for(int i = 0; i < ehdr->e_phnum; i++){
  //   if(phdr[i].p_type == PT_LOAD && phdr[i].p_vaddr < ehdr->e_entry && phdr[i].p_vaddr + phdr[i].p_memsz > ehdr->e_entry){
  //       Req_Prog_Header = phdr[i];
  //       index2 = i;
  //       flag = 1;
  //       break; // break when we know that the segment is of type PT_LOAD and when we know that entrypoint lies in this segment
  //   }
  // }

  // if(flag == 0){
  //   printf("PT_LOAD Not Found.\n");
  //   exit(1);
  // }

  // 3. Allocate memory of the size "p_memsz" using mmap function 
  //    and then copy the segment content

  // void* virtual_mem =  mmap((void*)phdr[index2].p_vaddr,page_size,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,-1,0);
  // lseek(fd,phdr[index2].p_offset,SEEK_SET);
  // read(fd, virtual_mem,phdr[index2].p_memsz);
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step

  // void* address =  virtual_mem + (ehdr->e_entry - phdr[index2].p_vaddr);
  void* address =  (void*)ehdr->e_entry;

  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.

  int (*_start)() = (int(*)())(address);

  // // 6. Call the "_start" method and print the value returned from the "_start"

  int result = _start();
  printf("\n\n\nUser _start return value = %d\n",result);

  // if(munmap(virtual_mem,Req_Prog_Header.p_memsz) == -1){
  //   perror("ERROR");
  //   exit(1);
  // };
}
