#include <stdio.h>
#include <unistd.h>

void main(){
    int i = 0;
    while(1){
        printf("%d) singh\n",i);
        sleep(4);
        i++;
    }
}