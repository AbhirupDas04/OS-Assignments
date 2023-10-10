#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <features.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

typedef struct Process_Queue{
    int n_proc;
    int list_procs[100];
    sem_t lock;
    int active_flag;
}Proc_Queue;

char* trim(char* string, char* str){
    int entry_status = 1;
    int exit_status = 0;
    int index = 0;
    for(int i = 0; i < (int)strlen(string);i++){
        if(entry_status == 1 && string[i] != ' ' && string[i] != '\t' && string[i] != '\n'){
            entry_status = 0;
            exit_status = 1;
            str[index] = string[i];
            index++;
            continue;
        }
        if(exit_status == 1){
            if(string[i] == ' ' || string[i] == '\t' || string[i] == '\n'){
                int flag = 0;
                for(int j = i; j < (int)strlen(string); j++){
                    if(string[j] == ' ' || string[j] == '\t' || string[j] == '\n'){
                        continue;
                    }
                    else{
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0){
                    str[index] = '\0';
                    return str;
                }
                else{
                    str[index] = string[i];
                    index++;
                    continue;
                }
            }
            else if(string[i] == '\0'){
                str[index] = '\0';
                return str;
            }
            else{
                str[index] = string[i];
                index++;
                continue;
            }
        }
    }

    str[index] = '\0';
    return str;
}

char* forward_trim(char* string, char* str){
    int index = 0;
    int len = strlen(string);

    for(int i = 0; i < len; i++){
        if(string[i] == ' ' || string[i] == '\t' || string[i] == '\n' || string[i] == '\0'){
            if(index!=0){
                str[index] = string[i];
                index++;
            }
            continue;
        }
        else{
            str[index] = string[i];
            index++;
        }
    }

    if(index == 0){
        return NULL;
    }

    str[index] = '\0';
    return str;
}

void Escape_sequence(int signum){
    if(signum == SIGINT){
        _exit(0);
    }

    if(signum == SIGCHLD){
        int* n = 0;
        int pid = waitpid(-1,n,WCONTINUED);
        printf("%d\n",pid);
    }
}

void main(){
    time_t start, end;
    time(&start);
    sleep(10);
    time(&end);
    printf("Time diff = %ld", end - start);
    char* args[] = {"./test2",NULL};

    // int status = fork();

    // if(status == 0){
    //     execvp("./test2",args);
    // }
    // else if(status > 0){
    //     wait(NULL);
    //     printf("%d %d\n",status,kill(status,0));
    // }
}