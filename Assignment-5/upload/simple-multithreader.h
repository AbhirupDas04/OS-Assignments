#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main

// parallel_for accepts a C++11 lambda function and runs the loop body (lambda) in  
// parallel by using ‘numThreads’ number of Pthreads to be created by the simple-multithreader 
void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){

}

// This version of parallel_for is for parallelizing two-dimensional for-loops, i.e., an outter for-i loop and  
// an inner for-j loop. Loop properties, i.e. low, high are mentioned below for both outter  
// and inner for-loops. The suffixes “1” and “2” represents outter and inner loop properties respectively.  
void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)>  &&lambda, int numThreads){
    
}


