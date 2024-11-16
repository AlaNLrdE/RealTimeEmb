#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

#define COUNT  1000 /* Define the number of iterations for increment and decrement */

typedef struct
{
    int threadIdx; /* Index of the thread */
} threadParams_t;

/* POSIX thread declarations and scheduling attributes */
pthread_t threads[2]; /* Array to hold thread descriptors for two threads */
threadParams_t threadParams[2]; /* Array to hold thread parameters for two threads */

/* Unsafe global variable */
int gsum = 0;

/* Thread function to increment the global sum */
void *incThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp; /* Cast the parameter to threadParams_t */

    for(i = 0; i < COUNT; i++)
    {
        gsum = gsum + i; /* Increment the global sum */
        printf("Increment thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum); /* Print the result */
    }
    return NULL; /* Return NULL as the thread function must return a void pointer */
}

/* Thread function to decrement the global sum */
void *decThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp; /* Cast the parameter to threadParams_t */

    for(i = 0; i < COUNT; i++)
    {
        gsum = gsum - i; /* Decrement the global sum */
        printf("Decrement thread idx=%d, gsum=%d\n", threadParams->threadIdx, gsum); /* Print the result */
    }
    return NULL; /* Return NULL as the thread function must return a void pointer */
}

int main (int argc, char *argv[])
{
    int rc; /* Return code for pthread functions */
    int i = 0; /* Loop counter */

    /* Create the increment thread */
    threadParams[i].threadIdx = i; /* Set the thread index */
    pthread_create(&threads[i],   /* Pointer to thread descriptor */
                   (void *)0,     /* Use default attributes */
                   incThread,     /* Thread function entry point */
                   (void *)&(threadParams[i]) /* Parameters to pass in */
                  );
    i++;

    /* Create the decrement thread */
    threadParams[i].threadIdx = i; /* Set the thread index */
    rc = pthread_create(&threads[i], (void *)0, decThread, (void *)&(threadParams[i]));

    /* Wait for both threads to complete */
    for(i = 0; i < 2; i++)
        pthread_join(threads[i], NULL);

    /* Print completion message */
    printf("TEST COMPLETE\n");

    return rc; /* Return success */
}