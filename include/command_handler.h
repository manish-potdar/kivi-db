#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>

#define MAX_RESPONSE_SIZE 256

// Struct for parser output
typedef struct {
    bool success;             // Indicates if the command was successfully executed
    char data[MAX_RESPONSE_SIZE]; // The response to be sent back
    char error[MAX_RESPONSE_SIZE];    // Error message (if any)
    bool exit; // to indicate client exit
} CommandResponse;

CommandResponse parse_command(const char* input);

#endif // END COMMAND PARSER
