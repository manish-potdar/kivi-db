// src/database.c
#include "../include/database.h"
#include <sqlite3.h>
#include <stdio.h>

#define DATABASE "kivi.db"

sqlite3 *db = NULL;  // Define the db variable

int initialize_database()
{
    const char *create_table_query =
        "CREATE TABLE IF NOT EXISTS key_value_store ("
        "key TEXT PRIMARY KEY, "
        "value TEXT);";

    // Open the database connection
    if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    char *errmsg = NULL;
    if (sqlite3_exec(db, create_table_query, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        fprintf(stderr, "Error creating table: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }

    return 0;
}

void close_database()
{
    if (db != NULL)
    {
        sqlite3_close(db);  // Close the database connection
        db = NULL;
    }
}
