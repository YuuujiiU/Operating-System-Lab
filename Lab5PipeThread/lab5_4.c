#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define BUF_LEN 6
int n;
int IN;
int OUT;
long int buf[BUF_LEN];
    
    
//check the format of the argument, only digit char allowed
int checkDigit(char *str){
    for(int i = 0;i<strlen(str);i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

// a recursive function that returns the factorial of a given number (n!)
long int factorial(int n)
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

void* ChildThread(void* pvar) {

        int ii = 0, interval;
        
        // set the seed
        srand((unsigned) time(NULL));
        
        while(ii < n){
             // wait for a random interval of time (0 to 2.999 seconds)
            interval = rand() % 3000;
            usleep(interval * 1000);

            // wait till buffer is not full
            while((IN + 1) % BUF_LEN == OUT);
            
            // produce an item
            buf[IN] = factorial(ii);
            
            //increment the index using module arithmatic
            IN = (IN + 1) % BUF_LEN;
           
            ii++;
        }
        
}

int main(int argc, char *argv[])
{
    
    // validate the input parameter n
    if(argc != 2){
        printf("usage: ./lab5_4 <n> \n");
        return -1;
    }

    // validate and obtain the input
    if(!checkDigit(argv[1])){
        printf("integer parameters only!\n");
        return -1;
    }    
    n = atoi(argv[1]);
    

    // initialize indices
    IN = 0;
    OUT = 0;

    // initialize a thread
    pthread_t thread_id;

    pthread_attr_t attr;
    pthread_attr_init(&attr);


    // Create the child thread
    pthread_create(&thread_id, &attr, ChildThread, (void *) NULL);
    
    // parent process field
        int ii = 0;
        
        while(ii < n){
            // wait till buffer is not empty
            while(IN == OUT);
            
            // consume an item
            printf("%ld ", buf[OUT]);
            fflush(stdout);

            // increment the read index using modulo arithmatic
            OUT = (OUT + 1) % BUF_LEN; 
            ii++;
        }
        
        printf("\n");
        fflush(stdout);

    //Wait for the child thread to exit
    pthread_join(thread_id, NULL);

    return 0;
}