#include "../include/command_handler.h"
// #include <cstdio>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// In-memory key-value store
typedef struct {
  char key[256];
  char value[256];
} KeyValuePair;

static KeyValuePair store[10];
static int store_size = 0;

CommandResponse parse_command(const char *input) {
  char command[256] = {0};
  char key[256] = {0};
  char value[256] = {0};

  CommandResponse response = {.success = false, .exit = false};

  // Parse the input command
  int args = sscanf(input, "%s %s %[^\r\n]", command, key, value);
  printf("command: %s\n", command);
  printf("key: %s\n", key);
  printf("value: %s\n", value);

  if (strcmp(command, "set") == 0 && args == 3) {
    // Handle "set key value"
  } else if (strcmp(command, "get") == 0 && args >= 2) {
    // Handle "get key"
  } else if (strcmp(command, "delete") == 0 && args >= 2) {
    // Handle "delete key"
  } else if (strcmp(command, "exit") == 0) {
    // Handle "exit"
    strcpy(response.error, "Goodbye!\r\n");
    response.exit = true;
    response.success = false;
    response.data[0] = '\0'; // indicating no data to return

    // return response; // Signal to close the connection
  } else {
    // invalid command
    strcpy(response.error, "Error: Invalid command.\n");
    response.success = false;
    response.data[0] = '\0'; // indicating no data to return
    response.exit = false;

    // return false;
  }

  return response;
}
