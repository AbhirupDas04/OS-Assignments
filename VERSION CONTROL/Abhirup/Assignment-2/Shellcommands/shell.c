#include "shell.h"

//PATH_MAX wasnt working so here is a quick fix

// #ifndef PATH_MAX
// #define PATH_MAX 4096
// #endif

#define PATH_MAX 4096

//main execution loop
char user_input[100][80];
int curr_idx =0;

void history(){
    int i=0;
    while(strncmp(user_input[i],"\0", strlen(user_input[i]))){
        printf("%d. ", i+1);
        yellow(user_input[i]);
        printf("\n");
        i++;
    }
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
            execl("/usr/bin/echo","/usr/bin/echo", arg, NULL);
            return 1;
        }

        //pwd
        else if (!strcmp(command,"pwd")){
            char cwd[PATH_MAX];
            getcwd(cwd,sizeof(cwd));

            if(getcwd(cwd,sizeof(cwd)) != NULL){
                blue(cwd);
                printf("\n");
                return 1;   
            }
            else{
                red("Error: pwd didnt work as expected!");
                printf("\n");
                return 1;
            }
            execl("/usr/bin/echo","/usr/bin/echo", arg, NULL);
            return 1;
        }

        //history
        else if(!strcmp(command,"history")){
            history();
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

    do{
        cyan("assignment2@shell:");
        magenta("~$ ");
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

        status = launch(command,arg);
    }
    
    while(status != 0);
    
}
