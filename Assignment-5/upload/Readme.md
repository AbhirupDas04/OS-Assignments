# Assignment-5 - Simple Multithreader
##### Abhirup Das | Armaan Singh



## File Structure
```
Assignment-5
|
|---upload
|    |---Makefile
|    |---Readme.md
|    |---matrix.cpp
|    |---simple-multithreader.h
|    |---simple-multithreader.pdf
|    |---vector.cpp
```

## Code Explanation
### Makefile
```
** No changes were made **

```
### matrix.cpp
```
** No changes were made **
```
### simple-multithreader.h
```
+----------------------------------------------+
|typedef struct {                              |
|    int loop_iter1;                           |
|    int low1;                                 |
|    int low2;                                 |
|    int high2;                                |
|    std::function<void(int)> fn1;             |
|    std::function<void(int, int)> fn2;        |
|} Lambda_Converter;                           |
|                                              |
|typedef struct {                              |
|    struct timespec start;                    |
|    struct timespec end;                      |
|    double diff_ms;                           |
|    int type;                                 |
|    int numThreads;                           |
|} Time_Controller;                            |
+----------------------------------------------+
Here we introduce two different structs namely, Lambda_Converter & Time_Controller
to organize all the informations regarding the lambda functions and also the time details respectively

+----------------------------------------------------------------------------------------------------------------------+
|void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads);                              |
|void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads);  |
+----------------------------------------------------------------------------------------------------------------------+
Here we create parallel_for() method to parallelize the execution of the lambda function using pthreads ( for one-dimensional loops only)
the second parallel_for() method is for handling two-dimensional loops

+----------------------------------------+
|void* fn_converter_1(void* converter);  |
|void* fn_converter_2(void* converter);  |
+----------------------------------------+
converts data from lambda to pthreads suitability

+-------------------------------------+
|Time_Controller timestruct_arr[20];  |
|int curr_tstruct_idx = -1;           |
+-------------------------------------+
 Array for storage of no. of 'parallel_for' calls

```
### vector.cpp
```
** No changes were made **
```

## Assumptions
```
- We are printing the time for each parallel_for and not for each thread
- We assume that (numthread -1) pthreads are to be created, since, total number of threads = main thread + no.of pthreads
[in point 2(b), it mentions that "SimpleMultithreader  runtime  execution  must  have  the  exact  number  of  threads  specified  by  the programmer including the main thread of execution.]
```
