#include <pthread.h> /* POSIX thread library */
#include <stdlib.h>  /* Standard library */
#include <stdio.h>   /* Standard I/O library */
#include <sched.h>   /* Scheduling library */
#include <syslog.h>  /* System logging library */
#include "logger.h" /* Header file for the logger */

#define NUM_THREADS 64 /* Define the number of threads */

/* Structure to hold thread parameters */
typedef struct
{
    int threadIdx; /* Index of the thread */
} threadParams_t;

/* POSIX thread declarations and scheduling attributes */
pthread_t threads[NUM_THREADS]; /* Array to hold thread descriptors */
threadParams_t threadParams[NUM_THREADS]; /* Array to hold thread parameters */

/* Thread function to calculate the sum of integers from 1 to threadIdx */
void *counterThread(void *threadp)
{
    int sum = 0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp; /* Cast the parameter to threadParams_t */

    /* Calculate the sum of integers from 1 to threadIdx */
    for(i = 1; i < (threadParams->threadIdx) + 1; i++)
        sum = sum + i;

    /* Print the result */
    printf("Thread idx=%d, sum[1...%d]=%d\n", 
           threadParams->threadIdx,
           threadParams->threadIdx, sum);

    return NULL; /* Return NULL as the thread function must return a void pointer */
}

int main (int argc, char *argv[])
{
    int rc; /* Return code for pthread functions */
    int i;  /* Loop counter */

    log_system_info();

    /* Create threads */
    for(i = 1; i < NUM_THREADS; i++)
    {
        threadParams[i].threadIdx = i; /* Set the thread index */

        /* Create a new thread */
        rc = pthread_create(&threads[i],   /* Pointer to thread descriptor */
                            NULL,          /* Use default attributes */
                            counterThread, /* Thread function entry point */
                            (void *)&(threadParams[i]) /* Parameters to pass in */
                           );

        if (rc) /* Check for errors */
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Wait for all threads to complete */
    for(i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    /* Print completion message */
    printf("TEST COMPLETE\n");

    return 0; /* Return success */
}