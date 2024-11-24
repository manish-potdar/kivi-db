// include/database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// Declare the database connection (db) as an extern variable
extern sqlite3 *db;

// Function declarations
int initialize_database();
void close_database();

#endif // DATABASE_H
