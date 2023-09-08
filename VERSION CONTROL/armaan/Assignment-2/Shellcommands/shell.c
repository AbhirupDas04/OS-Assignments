#include "shell.h"

//PATH_MAX wasnt working so here is a quick fix

// #ifndef PATH_MAX
// #define PATH_MAX 4096
// #endif

#define PATH_MAX 4096

//main execution loop
char user_input[100][80];
int curr_idx =0;
char exit_sequence[100][1000];
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

void history(){
    int i=0;
    while(strncmp(user_input[i],"\0", strlen(user_input[i]))){
        printf("%d. ", i+1);
        cyan(user_input[i]);
        printf("\n");
        i++;
    }
}

int launch(char command[30],char arg[50],int mode){
    int status = fork();

    if(status < 0){
        printf("Fork Failed");
        return 1;
    }

    else if(status == 0){
        if(mode == 0){
            char* args[50];
            char str[50];
            char* str2 = (char*)malloc(50*sizeof(char));
            char* token = strtok(command,"|");
            int i = 0;
            while(token != NULL){
                trim(token,str);
                strcpy(str2,str);
                args[i] = str2;
                str2 = str;
                str2 = (char*)malloc(50*sizeof(char));
                i++;
                token = strtok(NULL,"|");
            }
            args[i] = NULL;

            for(int j = 0; j < i-1; j ++){
                int fd[2];
                pipe(fd);

                if(fork() == 0) {
                    /* Child process */
                    dup2(fd[0],STDIN_FILENO);
                    close(fd[1]);

                    char arg3[50];
                    char *token1;
                    token1 = strtok(args[j+1], " ");
                    strcpy(command, token1);
                    token1 = strtok(NULL, "");
                    if(token1 != NULL) {
                        strcpy(arg3, token1);
                    }
                    else {
                        strcpy(arg3, "");
                    }

                    char* arg2[50];
                    token1 = strtok(arg3," ");
                    arg2[0] = args[j+1];
                    int i = 1;
                    while(token1 != NULL){
                        arg2[i] = token1;
                        i++;
                        token1 = strtok(NULL," ");
                    }
                    arg2[i] = NULL;

                    execvp(args[j+1],arg2);
                    exit(0); 
                }

                /* Parent process */
                dup2(fd[1],STDOUT_FILENO);
                close(fd[0]);

                char arg3[50];
                char *token1;
                token1 = strtok(args[j], " ");
                strcpy(command, token1);
                token1 = strtok(NULL, "");
                if(token1 != NULL) {
                    strcpy(arg3, token1);
                }
                else {
                    strcpy(arg3, "");
                }

                char* arg2[50];
                token1 = strtok(arg3," ");
                arg2[0] = args[j];
                int i = 1;
                while(token1 != NULL){
                    arg2[i] = token1;
                    i++;
                    token1 = strtok(NULL," ");
                }
                arg2[i] = NULL;

                execvp(args[j],arg2);
                wait(NULL);
            }

            return 1;
        }
        // ------------------------------------Extra-------------------------------------
        // //cd
        // else if(!strcmp(command,"cd")){
        //     //execl("/bin/cd","/bin/cd",arg,NULL);
        //     //this didnt work read abut it online it inbuilt function no exec file for it
        //     //so here c implemenation for it
        //     if (chdir(arg) != 0) {
        //         perror("cd");
        //     }
        //     execl("/bin/pwd","/bin/pwd",NULL);
        //     //now i cant show the complete path onto the prompt so i will just run pwd after each successful cd
        //     return 1;
        // }

        char* main_str = (char*)malloc(100);
        char* str1 = "which ";
        char* str2 = " > /dev/null 2>&1";
        int curr_index = 0;
        for(int i = 0; i < (int)strlen(str1); i++){
            curr_index++;
            main_str[i] = str1[i];
        }
        for(int i = 0; i < (int)strlen(command); i++){
            main_str[curr_index] = command[i];
            curr_index++;
        }
        for(int i = 0; i < (int)strlen(str2); i++){
            main_str[curr_index] = str2[i];
            curr_index++;
        }
        if(system(main_str)){
            printf("Command: \"%s\" not found.\n",command); 
        }
        else{
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
            execvp(command,args);
        }
        
        return 1;
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
        magenta("assignment2@shell:");
        cyan("~");
        yellow(cwd);
        white("$ ");
        fgets(input,100,stdin);
        input[strcspn(input,"\n")] = 0;

        if (!strcmp(input,"")){continue;}

        strncpy(user_input[curr_idx], input, 80); 
        curr_idx++;

        if(strstr(input,"|")==NULL){
            char *token;
            token = strtok(input, " ");
            strcpy(command, token);
            token = strtok(NULL, "");
            if(token != NULL) {
                strcpy(arg, token);
            }
            else {
                strcpy(arg, "");
            }
            int pid =getpid();
            clock_t st = clock();
            time_t start_time;
            time(&start_time);
            status = launch(command,arg,1);
            clock_t et = clock();
            time_t end_time;
            time(&end_time);
            double duration = (double)et - st / CLOCKS_PER_SEC;
            sprintf(exit_sequence[curr_idx-1], "Command \"%s\" executed by \n\tpid: %d\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %f ms", user_input[curr_idx-1], pid, ctime(&start_time), ctime(&end_time), duration);
        }
        else{
            int pid =getpid();
            clock_t st = clock();
            time_t start_time;
            time(&start_time);
            status = launch(input,arg,0);
            clock_t et = clock();
            time_t end_time;
            time(&end_time);
            double duration = (double)et - st / CLOCKS_PER_SEC;
            sprintf(exit_sequence[curr_idx-1], "Command \"%s\" executed by \n\tpid: %d\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %f ms", user_input[curr_idx-1], pid, ctime(&start_time), ctime(&end_time), duration);
        }
    }
    
    while(status != 0);
    
}