// #define _POSIX_SOURCE
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>

int dummy_main(int argc, char **argv);

int main(int argc, char **argv) {
	/* You can add any code here you want to support your SimpleScheduler implementation*/
	
	// if(kill(getpid(),19) == -1){
	// 	perror("ERROR");
	// 	exit(1);
	// }
	
	int ret = dummy_main(argc, argv);
	return ret;
}

#define main dummy_main
