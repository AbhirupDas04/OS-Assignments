#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//char user_input[100][80];
//int curr_idx =0;
//int launch(char command[30],char arg[50]){
//    system
//}

/*For colors we are goin to use the ANSI escape code
Black:30
Red:31
Green:32
Yellow:33
Blue:34
Magenta:35
Cyan:36
White:37
Reset:0
*/
//Here each function would print the text  provided in the specified color

void black(char* s){
    //black=30
    printf("\033[0;30m");
    printf("%s",s);
    printf("\033[0m");
}

void red(char* s){
    //red=31
    printf("\033[0;31m");
    printf("%s",s);
    printf("\033[0m");
}

void green(char* s){
    //green=32
    printf("\033[0;32m");
    printf("%s",s);
    printf("\033[0m");
}

void yellow(char* s){
    //yellow=33
    printf("\033[0;33m");
    printf("%s",s);
    printf("\033[0m");
}

void blue(char* s){
    //blue=34
    printf("\033[0;34m");
    printf("%s",s);
    printf("\033[0m");
}

void magenta(char* s){
    //magenta=35
    printf("\033[0;35m");
    printf("%s",s);
    printf("\033[0m");
}

void cyan(char* s){
    //cyan=36
    printf("\033[0;36m");
    printf("%s",s);
    printf("\033[0m");
}

void white(char* s){
    //white=37
    printf("\033[0;37m");
    printf("%s",s);
    printf("\033[0m");
}
int main(){ 
    //int status = 1;
    char input[100];
    //char command[30];
    //char arg[50];
    do{
        cyan("assignment2@shell:");
        magenta("~$ ");
        fgets(input,100,stdin);
        input[strcspn(input,"\n")] = 0;
        if (!strcmp(input,"")){continue;}
        system(input);
    }while(1);
        //strncpy(user_input[curr_idx], input, 80); 
        /*char *token;
    token = strtok(input, " ");
    strcpy(command, token);

    token = strtok(NULL, " ");
    while(token != NULL && (token[0] == '-' || (token[0] == '/' && token[1] == '/'))) {
    strcat(command, " ");
    strcat(command, token);
    token = strtok(NULL, " ");
    }

    if(token != NULL) {
    strcpy(arg, token);
    token = strtok(NULL, "");
    if(token != NULL) {
        strcat(arg, " ");
        strcat(arg, token);
    }
    } else {
        strcpy(arg, "");
    }
    launch(command,arg);
    }while(status != 0);
}*/
}