#include "../include/log.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <locale.h>

void log_message(const char* format, ...) {

    setlocale(LC_ALL, "en_US.UTF-8");
    // Get the current time
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[20]; // to hold formatted time string

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format time as [hh:mm:ss AM/PM]
    strftime(time_str, sizeof(time_str), "%d/%m/%Y %I:%M:%S %p", timeinfo);

    // Prepare the final log message
    va_list args;
    va_start(args, format);
    
    // Print the time and log message
    printf("[%s] ", time_str);
    vprintf(format, args); // pass the arguments to the printf
    va_end(args);
}
