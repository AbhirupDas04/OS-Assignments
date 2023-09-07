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
        cyan(user_input[i]);
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
            char *args[50];
            char *token = strtok(arg," ");
            int i =0;
            while(token != NULL){
                args[i] = token;
                i++;
                token = strtok(NULL," ");
            }
            args[i] = NULL;
            //execvp will take in variable arguments unlike excel
            printf("\n%s\n",args[1]);
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
            execl("/bin/grep","/bin/grep",arg,NULL);
            return 1;
        }
        //history
        else if(!strcmp(command,"history")){
            history();
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

    do{
        char cwd[PATH_MAX];
        getcwd(cwd,sizeof(cwd));
        magenta("assignment2@shell:");
        cyan("~");
        yellow(cwd);
        white("$ ");
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
