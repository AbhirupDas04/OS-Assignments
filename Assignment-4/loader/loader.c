#include "loader.h"

Elf32_Phdr *phdr;
Elf32_Ehdr* ehdr;
void* virtual_mem;
int fd;
int page_size;

int n_page_faults;
int n_page_allocs;
int total_frag;

int addr_array[1000];
int addr_idx = 0;

void Escape_sequence(int signum, siginfo_t *info){
    if(signum == SIGSEGV){
        int index2;
        Elf32_Phdr Req_Prog_Header;

        n_page_faults++;

        if(lseek(fd, 0, SEEK_SET) == -1){
            if(write(1,"Lseek Failed!\n",14)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        int flag = 0;
        for(int i = 0; i < ehdr->e_phnum; i++){
            if((void*)phdr[i].p_vaddr <= info->si_addr && (void*)phdr[i].p_vaddr + phdr[i].p_memsz >= info->si_addr){
                flag = 1;
                Req_Prog_Header = phdr[i];
                index2 = i;
                break; // break when we know that the segfault lies in this segment
            }
        }

        if(flag == 0){
            if(write(1,"Seg Fault not occurring in any segment!\n",28) == -1){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        void* start_page = info->si_addr - (int)info->si_addr % page_size;
        int offset = start_page - (void*)phdr[index2].p_vaddr;

        virtual_mem =  mmap((void*)phdr[index2].p_vaddr + offset,page_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0); // fd = -1 because MAP_ANONYMOUS is present
        if(virtual_mem == (void*)-1){
            if(write(1,"MMAP Failed!\n",13)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        addr_array[addr_idx++] = (int)((void*)phdr[index2].p_vaddr + offset);
        n_page_allocs ++;

        if(lseek(fd,phdr[index2].p_offset + offset,SEEK_SET) == -1){
            if(write(1,"Lseek Failed!\n",14)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        int n_bytes_read;
        if(phdr[index2].p_memsz - offset < page_size){
            n_bytes_read = phdr[index2].p_memsz - offset;
        }
        else{
            n_bytes_read = page_size;
        }

        if(read(fd, virtual_mem,n_bytes_read) == -1){
            if(write(1,"Read Failed!\n",13)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        total_frag += (page_size - n_bytes_read);
    }
}

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
    for(int i = 0; i < addr_idx; i++){
        if(munmap((void*)addr_array[i], page_size) == -1){
            perror("MUNMAP");
            exit(0);
        }
    }

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
    if(sigaction(SIGSEGV, &sig_struct, NULL) == -1){
        perror("SIGACTION");
        exit(EXIT_FAILURE);
    }

    page_size = getpagesize();

    // 1. Load entire binary content into the memory from the ELF file.

    ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr)); // For allocating memory for ELF Header
    if(ehdr == NULL){
        printf("Malloc Failed!\n");
        exit(EXIT_FAILURE);
    }

    ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr));
    if(NBytes_Read_EHDR == -1){
        perror("ERROR");
        exit(1);
    }


    phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum)); // For allocating memory for Program Header Table
    if(phdr == NULL){
        printf("Malloc Failed!\n");
        exit(EXIT_FAILURE);
    }

    ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum));
    if(NBytes_Read_PHDR == -1){
        perror("ERROR");
        exit(1);
    }

    // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step

    void* address =  (void*)ehdr->e_entry;

    // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.

    int (*_start)() = (int(*)())(address);

    // // 6. Call the "_start" method and print the value returned from the "_start"

    int result = _start();

    printf("\n\n\nUser _start return value = %d\n\n",result);

    double total_frag_double = total_frag / 1024.0;

    printf("Total number of page faults: %d\n",n_page_faults);
    printf("Total number of page allocations: %d\n",n_page_allocs);
    printf("Total amount of internal fragmentation: %.3f KB (Rounded off to 3 digits) = %d Bytes\n",total_frag_double,total_frag);
}
