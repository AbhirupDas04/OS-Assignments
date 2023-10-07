#include <stdio.h>
#include <stdlib.h>
#include "Shellcommands/shell.h" 

int main(int argc, char** argv){ 
    if(argc!=3){
        printf("Incorrect number of arguments!\n");
        exit(1);
    }

    int NCPU = atoi(argv[1]);
    if(NCPU <= 0){
        printf("Wrong value of NCPU.\n");
        exit(1);
    }

    int TSLICE = atoi(argv[2]);
    if(TSLICE <= 0){
        printf("Wrong value of Time Slice.\n");
        exit(1);
    }

    shell_loop(NCPU,TSLICE);
    return 0; 
}