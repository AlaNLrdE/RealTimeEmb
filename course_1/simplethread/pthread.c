#include <pthread.h> /* POSIX thread library */
#include <stdlib.h>  /* Standard library */
#include <stdio.h>   /* Standard I/O library */
#include <sched.h>   /* Scheduling library */
#include <syslog.h>  /* System logging library */

#define NUM_THREADS 1 /* Define the number of threads */

/* Structure to hold thread parameters */
typedef struct
{
    int threadIdx; /* Index of the thread */
} threadParams_t;

/* POSIX thread declarations and scheduling attributes */
pthread_t threads[NUM_THREADS]; /* Array to hold thread descriptors */
threadParams_t threadParams[NUM_THREADS]; /* Array to hold thread parameters */

void log_uname(void) 
{
    FILE *fp;
    const char* cmd = "uname -a"; // Command to get system information
    char output[1024]; // Buffer to store the command output

    // Execute the command and open a pipe to read its output
    fp = popen(cmd, "r");
    if (fp == NULL) 
    {
        // If the command fails, print an error message and exit
        printf("Failed to run command\n");
        exit(1);
    }
    
    // Read the output of the command line by line
    while (fgets(output, sizeof(output), fp)) 
    {
        // Log each line of the output to the syslog
        syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] %s", output);
    }

    // Close the pipe
    pclose(fp);
}

/* Thread function to calculate the sum of integers from 1 to threadIdx */
void *counterThread(void *threadp)
{
    int sum = 0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp; /* Cast the parameter to threadParams_t */

    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!");

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
    
    /* Open a connection to the syslog */
    openlog("simplethread", LOG_PID | LOG_CONS, LOG_USER);

    /* Clear the syslog */
    if (system("sudo truncate -s 0 /var/log/user.log") == -1) {
        syslog(LOG_ERR, "Failed to clear syslog");
        exit(EXIT_FAILURE);
    }
    log_uname();
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!");

    /* Create threads */
    for(i = 0; i < NUM_THREADS; i++)
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
    closelog(); /* Close the syslog connection */

    return 0; /* Return success */
}