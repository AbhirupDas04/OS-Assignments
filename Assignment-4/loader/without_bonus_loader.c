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

// This is our signal handler. The 'info' parameter is used to access the exact address where the segfault is occurring.
void Escape_sequence(int signum, siginfo_t *info){
    if(signum == SIGSEGV){
        int index2;
        Elf32_Phdr Req_Prog_Header;

        n_page_faults++;

        int flag = 0;
        for(int i = 0; i < ehdr->e_phnum; i++){
            if((void*)phdr[i].p_vaddr <= info->si_addr && (void*)phdr[i].p_vaddr + phdr[i].p_memsz >= info->si_addr){ // Checking if the segfault lies in this segment.
                flag = 1;
                Req_Prog_Header = phdr[i];
                index2 = i;
                break;
            }
        }

        if(flag == 0){ // If the seg fault address is not in any segment, we exit the program as that is not supposed to happen.
            if(write(1,"Seg Fault not occurring in any segment!\n",28) == -1){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        void* start_page = info->si_addr - (int)info->si_addr % page_size; // Attaining the starting address of the page that contains the segfault.
        int offset = start_page - (void*)phdr[index2].p_vaddr; // Finding the difference between the page and the starting of the segment that contains the page

        int curr_page_size = 0;
        while(curr_page_size < phdr[index2].p_memsz){
            curr_page_size += page_size;
        }

        virtual_mem =  mmap((void*)phdr[index2].p_vaddr,curr_page_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0); // fd = -1 because MAP_ANONYMOUS is present
        if(virtual_mem == (void*)-1){
            if(write(1,"MMAP Failed!\n",13)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        addr_array[addr_idx++] = (int)((void*)phdr[index2].p_vaddr); // Writing the address where I am mmap'ing to an array so that i can munmap the memory later on.
        n_page_allocs += (curr_page_size / page_size);

        if(lseek(fd,phdr[index2].p_offset,SEEK_SET) == -1){ // Moving the cursor of the file to the beginning of the page carrying the segfault.
            if(write(1,"Lseek Failed!\n",14)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        int n_bytes_read = phdr[index2].p_memsz;

        if(read(fd, virtual_mem,n_bytes_read) == -1){ // Reading the page from the file to the location where we are mmapping a page.
            if(write(1,"Read Failed!\n",13)){
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        total_frag += (curr_page_size - n_bytes_read); // Calculating the total internal fragmentation for this page.
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
    sig_struct.sa_flags = SA_SIGINFO; // For getting the address at which the segfault takes place.
    if(sigaction(SIGSEGV, &sig_struct, NULL) == -1){ // Initializing the signal handler with the corresponding signal.
        perror("SIGACTION");
        exit(EXIT_FAILURE);
    }

    page_size = getpagesize(); // To get the size of the page (Usually 4096 bytes)

    // 1. Load entire binary content into the memory from the ELF file.

    ehdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr)); // For allocating memory for ELF Header
    if(ehdr == NULL){
        printf("Malloc Failed!\n");
        exit(EXIT_FAILURE);
    }

    ssize_t NBytes_Read_EHDR = read(fd, ehdr, sizeof(Elf32_Ehdr)); // To copy the ELF Header contents into the struct for later use
    if(NBytes_Read_EHDR == -1){
        perror("ERROR");
        exit(1);
    }


    phdr = (Elf32_Phdr*)malloc((size_t)(ehdr->e_phentsize * ehdr->e_phnum)); // For allocating memory for Program Header Table
    if(phdr == NULL){
        printf("Malloc Failed!\n");
        exit(EXIT_FAILURE);
    }

    ssize_t NBytes_Read_PHDR = read(fd, phdr, (size_t)(ehdr->e_phentsize * ehdr->e_phnum)); // To copy the entire Program Header Table
    if(NBytes_Read_PHDR == -1){
        perror("ERROR");
        exit(1);
    }

    // Navigate to the entrypoint address into the segment

    void* address =  (void*)ehdr->e_entry;

    // Typecast the address to that of function pointer matching "_start" method in fib.c/sum.c .

    int (*_start)() = (int(*)())(address);

    // Call the "_start" method and print the value returned from the "_start"

    int result = _start();

    printf("\n\n\nUser _start return value = %d\n\n",result);

    double total_frag_double = total_frag / 1024.0; // Converting internal fragmentation bytes to KB.

    printf("Total number of page faults: %d\n",n_page_faults);
    printf("Total number of page allocations: %d\n",n_page_allocs);
    printf("Total amount of internal fragmentation: %.3f KB (Rounded off to 3 digits) = %d Bytes\n",total_frag_double,total_frag);
}
