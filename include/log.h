#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

// Function to log messages with timestamp in the format [hh:mm:ss AM/PM]
void log_message(const char* format, ...);

#endif // LOG_H
