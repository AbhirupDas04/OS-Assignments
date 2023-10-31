#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;
int index2;
void* address;
int page_size;
Elf32_Phdr Req_Prog_Header;
int count = 0;

// void Escape_sequence(int signum){
//     if(signum == SIGSEGV){
//       printf("%d\n",count);
//       address = mmap(NULL,ehdr->e_phentsize,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,0,0);
//       lseek(fd,phdr[index2].p_offset,SEEK_SET);
//       read(fd, address,ehdr->e_phentsize);

//       void* aunt = &phdr[index2] + (ehdr->e_entry - phdr[index2].p_vaddr);
//       int (*_start)() = (int(*)())(aunt);
//       int result = _start();
//       printf("User _start return value = %d\n",result);

//       address += page_size;
//       count++;
//     }
// }

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

  // if(signal(SIGSEGV,Escape_sequence) == SIG_ERR){
  //     perror("ERROR");
  //     exit(1);
  // }

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

  int flag = 0;

  for(int i = 0; i < ehdr->e_phnum; i++){
    if(phdr[i].p_type == PT_LOAD && phdr[i].p_vaddr < ehdr->e_entry && phdr[i].p_vaddr + phdr[i].p_memsz > ehdr->e_entry){
        Req_Prog_Header = phdr[i];
        index2 = i;
        flag = 1;
        break; // break when we know that the segment is of type PT_LOAD and when we know that entrypoint lies in this segment
    }
  }

  if(flag == 0){
    printf("PT_LOAD Not Found.\n");
    exit(1);
  }

  // // 3. Allocate memory of the size "p_memsz" using mmap function 
  // //    and then copy the segment content

  // void* virtual_mem = mmap(NULL, Req_Prog_Header.p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0,0);
  // if(virtual_mem == (void*)(-1)){
  //   printf("MMAP ERROR\n");
  //   exit(1);
  // }

  // if(lseek(fd,phdr[index2].p_offset,SEEK_SET) == -1){
  //   perror("ERROR");
  //   exit(1);
  // };

  // if(read(fd, virtual_mem,phdr[index2].p_memsz) == -1){
  //   perror("ERROR");
  //   exit(1);
  // };

  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step

  // address =  &phdr[index2] + (ehdr->e_entry - phdr[index2].p_vaddr);
  printf("%p\n",&Req_Prog_Header);
  Elf32_Phdr* a1 = &Req_Prog_Header;
  void* cat = (void*)a1;
  printf("%p\n",cat);

  // void* virtual_mem = mmap(NULL,page_size,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,0,0);
      void* virtual_mem =  mmap(cat,page_size,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,0,0);
      // memcpy(virtual_mem,&Req_Prog_Header,sizeof(Req_Prog_Header));
      // printf("%d\n",((Elf32_Phdr*)virtual_mem)->p_memsz);
      lseek(fd,phdr[index2].p_offset,SEEK_SET);
      read(fd, virtual_mem,Req_Prog_Header.p_memsz);
      void* address =  virtual_mem + (ehdr->e_entry - phdr[index2].p_vaddr);
      printf("%p\n",address);
      int (*_start)() = (int(*)())(address);
      int result = _start();
      printf("User _start return value = %d\n",result);

  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.

  // int (*_start)() = (int(*)())(address);

  // // 6. Call the "_start" method and print the value returned from the "_start"

  // int result = _start();
  // printf("User _start return value = %d\n",result);

  // if(munmap(virtual_mem,Req_Prog_Header.p_memsz) == -1){
  //   perror("ERROR");
  //   exit(1);
  // };
}
