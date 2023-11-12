#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// This Struct is to contain all the details regarding the lambda function and its parameters.
typedef struct{ 
    int loop_iter1; // This is to determine the outer loop's no.of iterations.
    int low1; // Starting index of outer loop
    int low2; // Starting index of inner loop
    int high2; // Ending index of inner loop
    std::function<void(int)> fn1;
    std::function<void(int,int)> fn2;
}Lambda_Converter;

// This Struct stores all the data with regards to the information about the time needed for the 'parallel_for'
// function.
typedef struct{
    struct timespec start; // Starting time
    struct timespec end; // Ending time
    double diff_ms; // Total Time taken in ms
    int type; // This is 1 if lambda fn of 1 parameter is used and 2 if 2 parameters
    int numThreads;
}Time_Controller;

Time_Controller timestruct_arr[20]; // Array for storage of no. of 'parallel_for' calls
int curr_tstruct_idx = -1;

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





    if(curr_tstruct_idx >= 0){
        printf("\n\nTime Taken by Each 'parallel_for' call:-\n\n");
        for(int i = 0; i <= curr_tstruct_idx; i++){
            printf("%d)\tType -> ", i + 1);
            if(timestruct_arr[i].type == 1){
                printf("Lambda Function with One Variable\n");
            }
            else{
                printf("Lambda Function with Two Variables\n");
            }

            printf("\tNo. of Threads -> %d\n",timestruct_arr[i].numThreads);


            char start_time_str[30], end_time_str[30];

            strftime(start_time_str, sizeof(start_time_str), "%H:%M:%S", localtime(&timestruct_arr[i].start.tv_sec)); // Converting the Time to a readable version
            strftime(end_time_str, sizeof(end_time_str), "%H:%M:%S", localtime(&timestruct_arr[i].end.tv_sec));
            printf("\tStart time -> %s.%3ld\n", start_time_str, timestruct_arr[i].start.tv_nsec);
            printf("\tEnd time -> %s.%3ld\n", end_time_str, timestruct_arr[i].end.tv_nsec);
            printf("\n\tTotal Time Taken -> %.3f seconds\n\n\n",timestruct_arr[i].diff_ms/1000);
        }
        printf("----------------------------------------------------------------------------\n");
    }

    return rc;
}

#define main user_main

// This function is used to convert the function needed to compute the lambda function of 1 variable to
// that of a function that matches pthread_create's function pointer signature.
void* fn_converter_1(void* converter) {
    Lambda_Converter* Main_Converter = (Lambda_Converter*)converter;
    std::function<void(int)> lambda = Main_Converter->fn1;
    for(int j = Main_Converter->low1; j < Main_Converter->low1 + Main_Converter->loop_iter1; j++){
        lambda(j);
    }
    return NULL;
}

// This function is used to convert the function needed to compute the lambda function of 2 variables to
// that of a function that matches pthread_create's function pointer signature.
void* fn_converter_2(void* converter) {
    Lambda_Converter* Main_Converter = (Lambda_Converter*)converter;
    std::function<void(int,int)> lambda = Main_Converter->fn2;
    for(int j = Main_Converter->low1; j < Main_Converter->low1 + Main_Converter->loop_iter1; j++){
        for(int l = Main_Converter->low2; l < Main_Converter->high2; l++){
            lambda(j,l);
        }
    }
    return NULL;
}

// parallel_for accepts a C++11 lambda function and runs the loop body (lambda) in  
// parallel by using ‘numThreads’ number of Pthreads to be created by the simple-multithreader 
void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start); // Measuring Starting Time

    struct timespec sleep_time;
    sleep_time.tv_sec = 4;
    sleep_time.tv_nsec = 4 * 100000000;


    
    int size = high - low;
    numThreads = numThreads - 1;

    int chunk_size;
    int n_excess;
    if(size % numThreads == 0){ // If this is 0, that means that we can divide chunks equally.
        chunk_size = size / numThreads;
    }
    else{ // Otherwise, we have to break chunks into slightly unequal pieces with some having 1 more iteration than the other.
        chunk_size = size / numThreads;
        n_excess = size % numThreads;
    }

    int start_idx = low;
    pthread_t thread_id_arr[numThreads];
    int loop_iter;
    for (int i=1; i <= numThreads; i++) {
        if(n_excess > 0){
            loop_iter = chunk_size + 1; // Determining the number of times the lambda function will be called inside current chunk.
            n_excess--;
        }
        else{
            loop_iter = chunk_size;
        }

        Lambda_Converter* l1 = new Lambda_Converter; // This stores all the data regarding the function.
        l1->low1 = start_idx;
        l1->loop_iter1 = loop_iter;
        l1->fn1 = lambda;

        pthread_create(&thread_id_arr[i-1], NULL, fn_converter_1, (void*)l1); // Creating the Threads.

        start_idx += loop_iter;
    }

    for (int i=1; i <= numThreads; i++) {
        pthread_join(thread_id_arr[i-1] , NULL); // Waiting for the threads to finish Execution
    }



    clock_gettime(CLOCK_REALTIME, &end); // Getting the end time.

    long long diff_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    double diff_ms = (double)diff_ns / 1000000.0;

    Time_Controller t1; // Storing data related to the timing of the fn call.
    t1.diff_ms = diff_ms;
    t1.end = end;
    t1.numThreads = numThreads;
    t1.start = start;
    t1.type = 1;
    timestruct_arr[++curr_tstruct_idx] = t1;
}

// This version of parallel_for is for parallelizing two-dimensional for-loops, i.e., an outter for-i loop and  
// an inner for-j loop. Loop properties, i.e. low, high are mentioned below for both outter  
// and inner for-loops. The suffixes “1” and “2” represents outter and inner loop properties respectively.  
void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)>  &&lambda, int numThreads){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start); // Measuring Starting Time

    struct timespec sleep_time;
    sleep_time.tv_sec = 4;
    sleep_time.tv_nsec = 4 * 100000000;



    int size = high1 - low1;
    numThreads = numThreads - 1;

    int chunk_size;
    int n_excess;
    if(size % numThreads == 0){ // If this is 0, that means that we can divide chunks equally.
        chunk_size = size / numThreads;
    }
    else{ // Otherwise, we have to break chunks into slightly unequal pieces with some having 1 more iteration than the other.
        chunk_size = size / numThreads;
        n_excess = size % numThreads;
    }

    int start_idx = low1;
    pthread_t thread_id_arr[numThreads];
    int loop_iter;
    for (int i=1; i <= numThreads; i++) {
        if(n_excess > 0){
            loop_iter = chunk_size + 1; // Determining the number of times the lambda function will be called inside current chunk.
            n_excess--;
        }
        else{
            loop_iter = chunk_size;
        }

        Lambda_Converter* l1 = new Lambda_Converter; // This stores all the data regarding the function.
        l1->low1 = start_idx;
        l1->loop_iter1 = loop_iter;
        l1->low2 = low2;
        l1->high2 = high2;
        l1->fn2 = lambda;

        pthread_create(&thread_id_arr[i-1], NULL, fn_converter_2, (void*)l1); // Creating the Threads.

        start_idx += loop_iter;
    }

    for (int i=1; i <= numThreads; i++) {
        pthread_join(thread_id_arr[i-1] , NULL); // Waiting for the threads to finish Execution
    }



    clock_gettime(CLOCK_REALTIME, &end); // Getting the end time.

    long long diff_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    double diff_ms = (double)diff_ns / 1000000.0;

    Time_Controller t1; // Storing data related to the timing of the fn call.
    t1.diff_ms = diff_ms;
    t1.end = end;
    t1.numThreads = numThreads;
    t1.start = start;
    t1.type = 2;
    timestruct_arr[++curr_tstruct_idx] = t1;
}