#include <stdio.h>
#include <stdlib.h>

int fib(int n) {
  if(n<2) return n;
  else return fib(n-1)+fib(n-2);
}

void main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
	int val = fib(n);
  printf("%d\n",val);
}