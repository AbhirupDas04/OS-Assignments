#include "shell.h"

#define PATH_MAX 4096

char user_input[100][80];
int curr_idx =0;
char exit_sequence[100][2000];

void Escape_sequence(int signum){
    if(signum == SIGINT){
        int i=0;
        if(write(1,"\n",1) == -1){
            _exit(1);
        }
        while(strncmp(exit_sequence[i],"\0", strlen(exit_sequence[i]))){
            int len = strlen(exit_sequence[i]);
            for (int j = 0; j < len; j++) {
                if(write(1, &exit_sequence[i][j], 1) == -1){
                    _exit(1);
                }
            }
            i++;
        }
        _exit(0);
    }

    if(signum == SIGCHLD){
        int* n = 0;
        waitpid(-1,n,WNOHANG);
    }
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

int lastBack(char* string){
  int len = (int)strlen(string);
  for(int i = len-1; i >=0; i--){
    if(string[i] == ' ' || string[i] == '\t' || string[i] == '\n'){
      continue;
    }
    else if(string[i] == '&'){
      return 1;
    }
    else{
      return 0;
    }
  }
  return 0;
}

void remAmp(char* old_str, char* new_str){
  int pos;

  int len = (int)strlen(old_str);
  for(int i = len-1; i >=0; i--){
    if(old_str[i] == '&'){
      pos = i;
      break;
    }
  }

  int i;

  for(i = 0; i < pos; i++){
    new_str[i] = old_str[i];
  }

  new_str[i] = '\0';
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
        printf("Fork Failed!");
        return 0;
    }

    else if(status == 0){
        if(mode == 0 || mode == 3){
            char* args[50];
            char str[50];
            char* str2 = (char*)malloc(50*sizeof(char));

            if(str2 == NULL){
                printf("Malloc failed!");
                exit(EXIT_FAILURE);
            }

            char* token = strtok(command,"|");

            int fd[2];
            int tmp = 0;

            int i = 0;
            while(token != NULL){
                trim(token,str);
                strcpy(str2,str);
                args[i] = str2;
                str2 = str;
                str2 = (char*)malloc(50*sizeof(char));

                if(str2 == NULL){
                    printf("Malloc failed!");
                    exit(EXIT_FAILURE);
                }

                i++;
                token = strtok(NULL,"|");
            }
            args[i] = NULL;

            int stdin_storage = 0;
            int stdout_storage = 1;

            for(int j = 0; j < i; j++){
                if(pipe(fd) == -1){
                    perror("ERROR");
                    exit(EXIT_FAILURE);
                }

                int pid = fork();
                if(pid < 0){
                    perror("ERROR");
                    exit(EXIT_FAILURE);
                }

                if(pid == 0){
                    if((stdin_storage = dup(STDIN_FILENO)) == -1){
                        perror("ERROR");
                        exit(EXIT_FAILURE);
                    }

                    if(dup2(tmp, 0) == -1){
                        perror("ERROR");
                        exit(EXIT_FAILURE);
                    }

                    if(j != i-1){
                        stdout_storage = dup(STDOUT_FILENO);
                        if(stdout_storage == -1){
                            perror("ERROR");
                            exit(EXIT_FAILURE);
                        }

                        if(dup2(fd[1], 1) == -1){
                            perror("ERROR");
                            exit(EXIT_FAILURE);
                        }
                    }

                    if(close(fd[0]) == -1){
                        perror("ERROR");
                        exit(EXIT_FAILURE);
                    }

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

                    if(execvp(args[j],arg2) == -1){
                        perror("ERROR");
                        exit(EXIT_FAILURE);
                    }

                    printf("Error: Execvp failed!");
                    exit(EXIT_FAILURE);
                }
                else{
                    int ret;
                    int pid = wait(&ret);

                    if(!WIFEXITED(ret)) {
                        printf("Abnormal termination of %d\n",pid);
                        exit(EXIT_FAILURE);
                    }

                    if(close(fd[1]) == -1){
                        perror("ERROR");
                        exit(EXIT_FAILURE);
                    }

                    tmp = fd[0];
                }
            }

            if(dup2(stdin_storage, STDIN_FILENO) == -1){
                perror("ERROR");
                exit(1);
            }
            if(dup2(stdout_storage,STDOUT_FILENO) == -1){
                perror("ERROR");
                exit(1);
            }

            free(str2);

            exit(0);
        }

        if(!strcmp(command,"history")){
            char temp[100];
            trim(arg,temp);
            if(strlen(temp) > 0){
                printf("Too many args!");
                exit(1);
            }
            history();
            exit(0);
        }

        char* main_str = (char*)malloc(100);
        if(main_str == NULL){
            printf("Malloc failed!");
            exit(EXIT_FAILURE);
        }

        char* str1 = "which ";
        char* str2 = " > /dev/null 2>.1";
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

        int val = system(main_str);
        if(val){
            printf("Command: \"%s\" not found.\n",command); 
            exit(0);
        }
        else if(val == -1){
            perror("ERROR");
            exit(EXIT_FAILURE);
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
            if(execvp(command,args) == -1){
                perror("ERROR");
                exit(EXIT_FAILURE);
            }

            printf("Exec Failed!\n");
            exit(EXIT_FAILURE);
        }

        free(main_str);
        
        exit(0);
    }

    else{
        int ret;

        if(mode != 2 && mode != 3){
            status = wait(&ret);

            if(!WIFEXITED(ret)) {
                printf("Abnormal termination of %d\n",status);
            }
        }

        return status;
    }
}

void shell_loop(){
    int status = 1;
    char input[100];
    char command[30];
    char arg[50];

    if(signal(SIGINT,Escape_sequence) == SIG_ERR){
        perror("ERROR");
        exit(1);
    }

    if(signal(SIGCHLD,Escape_sequence) == SIG_ERR){
        perror("ERROR");
        exit(1);
    }

    do{
        char cwd[PATH_MAX];
        if(getcwd(cwd,sizeof(cwd)) == NULL){
            perror("ERROR");
            exit(1);
        }
        magenta("assignment2@shell:");
        cyan("~");
        yellow(cwd);
        white("$ ");

        if(fgets(input,100,stdin) == NULL){
            printf("fgets has failed or there is nothing to input anymore!");
            exit(1);
        }

        input[strcspn(input,"\n")] = 0;
        if (!strcmp(input,"")){continue;}

        strncpy(user_input[curr_idx], input, 80); 

        char test_input[100];
        int flag_bg_detect = 0;
        if(lastBack(input) == 1){
            remAmp(input,test_input);
            flag_bg_detect = 1;
            strcpy(input,test_input);
        }

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

            if(!strcmp(command,"run")){
                FILE *fptr;

                fptr = fopen(arg, "r");
                if(fptr == NULL){
                    perror("ERROR");
                    continue;
                }

                long cursor;

                while(fgets(input, 100, fptr)) {
                    cursor = ftell(fptr);
                    if(cursor == -1){
                        printf("ftell failed!\n");
                        exit(1);
                    }

                    input[strcspn(input,"\n")] = 0;

                    if(!strcmp(input,"") || !strcmp(input,"\n")){
                        continue;
                    }

                    strncpy(user_input[curr_idx], input, 80); 

                    if(lastBack(input) == 1){
                        remAmp(input,test_input);
                        flag_bg_detect = 1;
                        strcpy(input,test_input);
                    }

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

                        struct timeval start, end;
                        time_t t,u;
                        struct tm* info1;
                        struct tm* info2;
                        char buffer[64], buffer2[64];

                        if(gettimeofday(&start, NULL) == -1){
                            perror("ERROR");
                            exit(1);
                        }

                        t = start.tv_sec;
                        info1 = localtime(&t);
                        if(info1 == NULL){
                            perror("ERROR");
                            exit(1);
                        }
                        strftime(buffer, sizeof buffer, "%A, %B %d - %H:%M:%S\n", info1);

                        if(flag_bg_detect == 1){
                            status = launch(command,arg,2);
                        }
                        else{
                            status = launch(command,arg,1);
                        }
                        
                        if(gettimeofday(&end, NULL) == -1){
                            perror("ERROR");
                            exit(1);
                        }

                        u = end.tv_sec;
                        info2 = localtime(&u);
                        if(info2 == NULL){
                            perror("ERROR");
                            exit(1);
                        }
                        strftime(buffer2, sizeof buffer2, "%A, %B %d - %H:%M:%S\n", info2);

                        double duration = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0;
                        if(sprintf(exit_sequence[curr_idx-1], "%d) Command \"%s\" executed by \n\tpid: %d\n\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %lf s\n\n",curr_idx, user_input[curr_idx-1], status, buffer, buffer2, duration) < 0){
                            printf("Sprintf failed!");
                            exit(1);
                        }
                    }

                    else{
                        struct timeval start, end;
                        time_t t,u;
                        struct tm* info1;
                        struct tm* info2;
                        char buffer[64], buffer2[64];

                        if(gettimeofday(&start, NULL) == -1){
                            perror("ERROR");
                            exit(1);
                        }

                        t = start.tv_sec;
                        info1 = localtime(&t);
                        if(info1 == NULL){
                            perror("ERROR");
                            exit(1);
                        }
                        strftime(buffer, sizeof buffer, "%A, %B %d - %H:%M:%S\n", info1);

                        // printf("\n\n\n\n%s\n\n\n\n",input);

                        if(flag_bg_detect == 1){
                            status = launch(input,arg,3);

                        }
                        else{
                            status = launch(input,arg,0);
                        }
                        
                        if(gettimeofday(&end, NULL) == -1){
                            perror("ERROR");
                            exit(1);
                        }

                        u = end.tv_sec;
                        info2 = localtime(&u);
                        if(info2 == NULL){
                            perror("ERROR");
                            exit(1);
                        }
                        strftime(buffer2, sizeof buffer2, "%A, %B %d - %H:%M:%S\n", info2);

                        double duration = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0;
                        if(sprintf(exit_sequence[curr_idx-1], "%d) Command \"%s\" executed by \n\tpid: %d\n\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %lf s\n\n",curr_idx, user_input[curr_idx-1], status, buffer, buffer2, duration) < 0){
                            printf("Sprintf failed!");
                            exit(1);
                        }
                    }

                    if(fseek(fptr,cursor,SEEK_SET) == -1){
                        perror("ERROR");
                        exit(1);
                    }
                    
                }
            
                fclose(fptr);
            }

            else{
                curr_idx++;
                struct timeval start, end;
                time_t t,u;
                struct tm* info1;
                struct tm* info2;
                char buffer[64], buffer2[64];

                if(gettimeofday(&start, NULL) == -1){
                    perror("ERROR");
                    exit(1);
                }

                t = start.tv_sec;
                info1 = localtime(&t);
                if(info1 == NULL){
                    perror("ERROR");
                    exit(1);
                }
                strftime(buffer, sizeof buffer, "%A, %B %d - %H:%M:%S\n", info1);

                if(flag_bg_detect == 1){
                    status = launch(command,arg,2);
                }
                else{
                    status = launch(command,arg,1);
                }

                if(gettimeofday(&end, NULL) == -1){
                    perror("ERROR");
                    exit(1);
                }

                u = end.tv_sec;
                info2 = localtime(&u);
                if(info2 == NULL){
                    perror("ERROR");
                    exit(1);
                }
                strftime(buffer2, sizeof buffer2, "%A, %B %d - %H:%M:%S\n", info2);

                double duration = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0;
                if(sprintf(exit_sequence[curr_idx-1], "%d) Command \"%s\" executed by \n\tpid: %d\n\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %lf s\n\n",curr_idx, user_input[curr_idx-1], status, buffer, buffer2, duration) < 0){
                    printf("Sprintf failed!");
                    exit(1);
                }
            }
        }
        else{
            curr_idx++;
            struct timeval start, end;
            time_t t,u;
            struct tm* info1;
            struct tm* info2;
            char buffer[64], buffer2[64];

            if(gettimeofday(&start, NULL) == -1){
                perror("ERROR");
                exit(1);
            }

            t = start.tv_sec;
            info1 = localtime(&t);
            if(info1 == NULL){
                perror("ERROR");
                exit(1);
            }
            strftime(buffer, sizeof buffer, "%A, %B %d - %H:%M:%S\n", info1);

            if(flag_bg_detect == 1){
                status = launch(input,arg,3);
            }
            else{
                status = launch(input,arg,0);
            }
            
            if(gettimeofday(&end, NULL) == -1){
                perror("ERROR");
                exit(1);
            }

            u = end.tv_sec;
            info2 = localtime(&u);
            if(info2 == NULL){
                perror("ERROR");
                exit(1);
            }
            strftime(buffer2, sizeof buffer2, "%A, %B %d - %H:%M:%S\n", info2);

            double duration = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0;
            if(sprintf(exit_sequence[curr_idx-1], "%d) Command \"%s\" executed by \n\tpid: %d\n\n\tStartTime: %s\n\tEndTime: %s\n\tDuration: %lf s\n\n",curr_idx, user_input[curr_idx-1], status, buffer, buffer2, duration) < 0){
                printf("Sprintf failed!");
                exit(1);
            }
        }
    }
    
    while(status != 0);
}