#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>

#define MAX_RESPONSE_SIZE 1024

// Struct for parser output
typedef struct {
    bool success;             // Indicates if the command was successfully executed
    char data[MAX_RESPONSE_SIZE]; // The response to be sent back
    char error[MAX_RESPONSE_SIZE];    // Error message (if any)
    bool exit; // to indicate client exit
} CommandResponse;

CommandResponse parse_command(const char* input);

int initialize_database();
void close_database();

//declare handler for commands
CommandResponse handle_set(const char *key, const char *value);
CommandResponse handle_update(const char *key, const char *value);
CommandResponse handle_get(const char *key);
CommandResponse handle_delete(const char *key);

#endif // END COMMAND PARSER
