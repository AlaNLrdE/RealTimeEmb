#include <stdio.h>   /* Standard I/O library */
#include <stdlib.h>  /* Standard library */
#include "logger.h"  /* Header file for the logger */

void log_system_info() {
    FILE *fp;
    FILE *log_fp;
    char buffer[128];

    // Open the command for reading
    fp = popen("uname -a", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        exit(1);
    }

    // Open the log file for writing
    log_fp = fopen("system.log", "w");
    if (log_fp == NULL) {
        perror("Failed to open log file");
        exit(1);
    }

    // Read the output a line at a time - output it to the log file
    while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
        fprintf(log_fp, "%s", buffer);
    }

    // Close the command and the log file
    pclose(fp);
    fclose(log_fp);
}