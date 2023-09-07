#include "shell.h"
#include<sys/time.h>
//PATH_MAX wasnt working so here is a quick fix

// #ifndef PATH_MAX
// #define PATH_MAX 4096
// #endif

#define PATH_MAX 4096

//main execution loop
char user_input[100][80];
int curr_idx =0;
char exit_sequence[100][1000];
void history(){
    int i=0;
    while(strncmp(user_input[i],"\0", strlen(user_input[i]))){
        printf("%d. ", i+1);
        yellow(user_input[i]);
        printf("\n");
        i++;
    }
}
void Escape_sequence(int signum){
    int i=0;
    printf("caught signal %d\n",signum);
    while(strncmp(exit_sequence[i],"\0", strlen(exit_sequence[i]))){
        printf("%d. ", i+1);
        yellow(exit_sequence[i]);
        printf("\n");
        i++;
    }
    exit(0);
}

int launch(char command[30],char arg[50]){
    int status = fork();

    if(status < 0){
        printf("Fork Failed");
        return 1;
    }

    else if(status == 0){
        //echo
        if (!strcmp(command,"echo")){
            execl("/bin/echo","/bin/echo", arg, NULL);
            return 1;
        }

        //pwd
        else if (!strcmp(command,"pwd")){
            execl("/bin/pwd","/bin/pwd",NULL);
            return 1;
        }
        //ls
        else if (!strcmp(command,"ls")){
            if (strlen(arg) > 0) {
                execl("/bin/ls","/bin/ls",arg,NULL);
            } else {
                execl("/bin/ls","/bin/ls",NULL);
            }
            return 1;
        }
        //wc
        else if (!strcmp(command,"wc")){  
            char* args[50];
            char *token = strtok(arg," ");
            args[0] = command;
            int i =1;
            while(token != NULL){
                args[i] = token;
                i++;
                token = strtok(NULL," ");
            }
            args[i] = NULL;
            //execvp will take in variable arguments unlike excel
            execvp("/bin/wc",args);
            return 1;
        }
        
        //sort
        else if(!strcmp(command,"sort")){
            execl("/bin/sort","/bin/sort",arg,NULL);
            return 1;
        }
        //uniq
        else if(!strcmp(command,"uniq")){
            execl("/bin/uniq","/bin/uniq",arg,NULL);
            return 1;
        }
        //cat
        else if(!strcmp(command,"cat")){
            execl("/bin/cat","/bin/cat",arg,NULL);
            return 1;
        }
        //grep
        else if(!strcmp(command,"grep")){
            char* args[50];
            char *token = strtok(arg," ");
            args[0] = command;
            int i =1;
            while(token != NULL){
                args[i] = token;
                i++;
                token = strtok(NULL," ");
            }
            args[i] = NULL;
            //execvp will take in variable arguments unlike execl
            execvp("/bin/grep",args);
            return 1;
        }
        //history
        else if(!strcmp(command,"history")){
            history();
            return 1;
        }
        // ./
        else if(!strncmp(command,"./",2)){
            char* args[50];
            char *token = strtok(arg," ");
            args[0] = command;
            int i =1;
            while(token != NULL){
                args[i] = token;
                i++;
                token = strtok(NULL," ");
            }
            args[i] = NULL;
            //execvp will take in variable arguments unlike excel
            execvp(command,args);
            return 1;
        }
        // ------------------------------------Extra-------------------------------------
        //cd
        else if(!strcmp(command,"cd")){
            //execl("/bin/cd","/bin/cd",arg,NULL);
            //this didnt work read abut it online it inbuilt function no exec file for it
            //so here c implemenation for it
            if (chdir(arg) != 0) {
                perror("cd");
            }
            execl("/bin/pwd","/bin/pwd",NULL);
            //now i cant show the complete path onto the prompt so i will just run pwd after each successful cd
            return 1;
        }
        else{
            printf("Command: \"%s\" not found.\n",command); 
            return 1;
        }
    }
    else{
        int ret;
        int pid = wait(&ret);
        if(!WIFEXITED(ret)) {
            printf("Abnormal termination of %d\n",pid);
        }
        return 1;
    }
}

void shell_loop(){
    int status = 1;
    char input[100];
    char command[30];
    char arg[50];
    signal(SIGINT,Escape_sequence);

    do{
        char cwd[PATH_MAX];
        getcwd(cwd,sizeof(cwd));
        yellow("assignment2@shell:");
        yellow("~");
        cyan(cwd);
        white("$");
        fgets(input,100,stdin);
        input[strcspn(input,"\n")] = 0;

        if (!strcmp(input,"")){continue;}

        strncpy(user_input[curr_idx], input, 80); 
        curr_idx++;

        //fgets(arg,50,stdin);
        //printf("\n");
        //printf("command:%s\narg:%s\n",command,arg);
        //status = strcmp(command,"exit");

        char *token;
        token = strtok(input, " ");
        strcpy(command, token);
        token = strtok(NULL, "");
        if(token != NULL) {
            strcpy(arg, token);
        } else {
            strcpy(arg, "");
        }
        int pid =getpid();
        clock_t st = clock();
        time_t start_time;
        time(&start_time);
        //printf("a\n");
        status = launch(command,arg);
        //printf("b\n");
        clock_t et = clock();
        time_t end_time;
        time(&end_time);
        //printf("%f",(double)et);
        //printf("c\n");
        double duration = (double)et - st / CLOCKS_PER_SEC;
        //found it online to like add formattedd string into a string array
        //printf("d\n");
        sprintf(exit_sequence[curr_idx-1], "Command \"%s\" executed by \n\tpid: %d\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %f ms", user_input[curr_idx-1], pid, ctime(&start_time), ctime(&end_time), duration);
        //printf("e\n");
    }while(status != 0);
    
}
