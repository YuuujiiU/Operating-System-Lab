#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

int64_t counter;
void *ChildThread(void *arg);
int n;

void *ChildThread(void *arg)
{
    int n = *(int *)arg;
    int i;

    for (i = 0; i < n; i++)
    {
        counter -= 2;
    }

    pthread_exit(0);
}

int main()
{
    // initialize a thread
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int status;
    counter = 0;

    printf("please type the value of n: \n");
    scanf("%d", &n);

    // Create the child thread
    status = pthread_create(&t, &attr, ChildThread, (void *)&n);
    if (status)
    {
        fprintf(stderr, "pthread_create() failed [status: %d]\n", status);
        return -1;
    }

    // parent process field
    int ii = 0;
    while(ii < n)
    {
        counter -= 2;
        ii++;
    }

    // Wait for the child thread to exit
    status = pthread_join(t, NULL);
    if (status)
    {
        fprintf(stderr, "pthread_join() failed [status: %d]\n", status);
        return -1;
    }

    printf("%ld\n", counter);
    return 0;
}
