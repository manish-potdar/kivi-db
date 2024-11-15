#include "../include/command_handler.h"
#include "../include/database.h"
// #include <cstdio>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

 // path to databse file

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

CommandResponse parse_command(const char *input)
{
  char command[256] = {0};
  char key[256] = {0};
  char value[256] = {0};

  CommandResponse response = {.success = false, .exit = false};

  // Parse the input command
  int args = sscanf(input, "%s %s %[^\r\n]", command, key, value);
  printf("command: %s\n", command);
  printf("key: %s\n", key);
  printf("value: %s\n", value);

  if (strcmp(command, "set") == 0 && args == 3)
  {
    // Handle "set key value"
    // const char *insert_query = "INSERT INTO key_value_store (key, value) VALUES (?, ?);";
    // sqlite3_stmt *stmt;

    // if (sqlite3_prepare_v2(db, insert_query, -1, &stmt, NULL) != SQLITE_OK)
    // {
    //   strcpy(response.error, "Error preparing database statement.\n");
    // }
    // else
    // {
    //   sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);
    //   sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);

    //   if (sqlite3_step(stmt) == SQLITE_DONE)
    //   {
    //     strcpy(response.data, "DONE\r\n");
    //     response.success = true;
    //   }
    //   else
    //   {
    //     snprintf(response.error, MAX_RESPONSE_SIZE, "Error: Key '%s' already exists.\r\n", key);
    //   }
    //   sqlite3_finalize(stmt);
    // }

    return handle_set(key, value);
  }
  else if (strcmp(command, "update") ==0 && args == 3){
    //handle update
    // const char *update_query = "UPDATE key_value_store SET value = ? WHERE key = ?;";
    // sqlite3_stmt *stmt;
    
    // if(sqlite3_prepare_v2(db, update_query, -1, &stmt, NULL) != SQLITE_OK)
    // {
    //   strcpy(response.error, "Error preparing database statement.\n");
    // }
    // else {
    //   //Biind new value
    //   sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 2, key, -1, SQLITE_STATIC);

    // // Execute the statement
    // if (sqlite3_step(stmt) == SQLITE_DONE) {
    //    // Check if any row was deleted
    //     if (sqlite3_changes(db) > 0)
    //     {
    //        strcpy(response.data, "Updated successfully.\r\n");
    //     response.success = true;
    //     }
    //     else
    //     {
    //       // No row matched the key
    //       strcpy(response.error, "Error: Key not found.\r\n");
    //       response.success = false;
    //     }
       
    // } else {
    //     strcpy(response.error, "Failed to update key.\n");
    // }

    // sqlite3_finalize(stmt);
    // }

    return handle_update(key, value);
  }
  else if (strcmp(command, "get") == 0 && args >= 2)
  {
    // Handle "get key"
    // const char *select = "SELECT value FROM key_value_store WHERE key = ?;";
    // sqlite3_stmt *stmt;

    // if (sqlite3_prepare_v2(db, select, -1, &stmt, NULL) != SQLITE_OK)
    // {
    //   strcpy(response.error, "Error preparing database statement.\n");
    // }
    // else
    // {
    //   sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);

    //   if (sqlite3_step(stmt) == SQLITE_ROW)
    //   {
    //     snprintf(response.data, MAX_RESPONSE_SIZE, "%s\n", sqlite3_column_text(stmt, 0));
    //     response.success = true;
    //     response.exit = false;
    //   }
    //   else
    //   {
    //     strcpy(response.error, "Key not found.\n");
    //   }
    //   sqlite3_finalize(stmt);
    // }
    return handle_get(key);
  }
  else if (strcmp(command, "delete") == 0 && args >= 2)
  {
    // Handle "delete key"
    // const char *delete_query = "DELETE FROM key_value_store WHERE key = ?;";
    // sqlite3_stmt *stmt;

    // if (sqlite3_prepare_v2(db, delete_query, -1, &stmt, NULL) != SQLITE_OK)
    // {
    //   strcpy(response.error, "Error preparing database statement.\n");
    // }
    // else
    // {
    //   sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);

    //   if (sqlite3_step(stmt) == SQLITE_DONE)
    //   {
    //     // Check if any row was deleted
    //     if (sqlite3_changes(db) > 0)
    //     {
    //       snprintf(response.data, MAX_RESPONSE_SIZE, "DELETED %s\r\n", key);
    //       response.success = true;
    //     }
    //     else
    //     {
    //       // No row matched the key
    //       strcpy(response.error, "Error: Key not found.\r\n");
    //       response.success = false;
    //     }
    //   }
    //   else
    //   {
    //     strcpy(response.error, "Internal Error\r\n");
    //     response.success = false;
    //   }
    //   sqlite3_finalize(stmt);
    // }
    return handle_delete(key);
  }
  else if (strcmp(command, "exit") == 0)
  {
    // Handle "exit"
    strcpy(response.error, "Goodbye!\r\n");
    response.exit = true;
    response.success = false;
    response.data[0] = '\0'; // indicating no data to return

    // return response; // Signal to close the connection
  }
  else
  {
    // invalid command
    strcpy(response.error, "Error: Invalid command.\n");
    response.success = false;
    response.data[0] = '\0'; // indicating no data to return
    response.exit = false;

    // return false;
  }

  return response;
}
