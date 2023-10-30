#include "loader.h"

int main(int argc, char** argv) 
{
  if(argc != 2) {
    printf("Usage: %s <ELF Executable> \n",argv[0]);
    exit(1);
  }
  // 1. carry out necessary checks on the input ELF file

  int fd;

  if((fd = open(argv[1], O_RDONLY))==-1){
    perror("ERROR");
    exit(1);
  };

  Elf32_Ehdr* ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
  ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr));
  if(NBytes_Read_EHDR == -1){
    perror("ERROR");
    exit(1);
  }

  if(ehdr->e_ident[0] != ELFMAG0 && ehdr->e_ident[1] != ELFMAG1 && ehdr->e_ident[2] != ELFMAG2 && ehdr->e_ident[3] != ELFMAG3){
    printf("Input file is not an ELF File.\n");
    exit(1);
  }

  if(ehdr->e_type != ET_EXEC){
    printf("Input file not an Executable file\n");
    exit(1);
  }

  if(ehdr->e_version == EV_NONE){
    printf("Input ELF file does not have the right Version\n");
    exit(1);
  }

  if(close(fd) == -1){
    perror("ERROR");
    exit(1);
  };

  free(ehdr);
  // 2. passing it to the loader for carrying out the loading/execution
  load_and_run_elf(&argv[1]);
  // 3. invoke the cleanup routine inside the loader  
  loader_cleanup();
  return 0;
}
