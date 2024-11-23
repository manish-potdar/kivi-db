#include "../include/command_handler.h"
#include "../include/database.h"
#include "../include/log.h"
// #include <cstdio>
#include <pthread.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

extern sqlite3 *db; // db connection

// initialize sqlite
// int initialize_database()
// {
//   const char *create_table_query =
//       "CREATE TABLE IF NOT EXISTS key_value_store ("
//       "key TEXT PRIMARY KEY, "
//       "value TEXT);";
//   // open db conn
//   if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
//   {
//     fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
//     return -1;
//   }

//   char *errmsg = NULL;
//   if (sqlite3_exec(db, create_table_query, NULL, NULL, &errmsg) != SQLITE_OK)
//   {
//     fprintf(stderr, "Error creating table: %s\n", errmsg);
//     sqlite3_free(errmsg);
//     return -1;
//   }

//   return 0;
// }

// close db conn
// void close_database()
// {
//   sqlite3_close(db);
// }

// // In-memory key-value store
// typedef struct {
//   char key[256];
//   char value[256];
// } KeyValuePair;

// static KeyValuePair store[10];
// static int store_size = 0;

CommandResponse parse_command(const char *input) {
  char command[256] = {0};
  char key[256] = {0};
  char value[256] = {0};

  CommandResponse response = {.success = false, .exit = false};
  pthread_mutex_t mutex;

  // Parse the input command
  int args = sscanf(input, "%s %s %[^\r\n]", command, key, value);
  log_message("command: %s\n", command);
  log_message("key: %s\n", key);
  log_message("value: %s\n", value);

  if (strcmp(command, "set") == 0 && args == 3) {

    return handle_set(key, value);
  } else if (strcmp(command, "update") == 0 && args == 3) {

    return handle_update(key, value);

  } else if (strcmp(command, "get") == 0 && args >= 2) {

    return handle_get(key);
  } else if (strcmp(command, "delete") == 0 && args >= 2) {

    return handle_delete(key);
  } else if (strcmp(command, "exit") == 0) {
    // Handle "exit"
    strcpy(response.error, "Goodbye!\r\n");
    response.exit = true;
    response.success = false;
    response.data[0] = '\0'; // indicating no data to return

    // return response; // Signal to close the connection
  } else if (strcmp(command, "SYNC_SET") == 0 && args == 3) {
    // handle sync_set command
    CommandResponse response = handle_sync_set(key, value);
    return response;
  } else if (strcmp(command, "SYNC_UPDATE") == 0 && args == 3) {
    // handle sync_update command
    CommandResponse response = handle_sync_update(key, value);
    return response;
  } else if (strcmp(command, "SYNC_DELETE") == 0 && args == 2) {
    // handle sync_delete command
    CommandResponse response = handle_sync_delete(key);
    return response;
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
