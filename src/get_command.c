#include "command_handler.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

extern sqlite3 *db; // Declare external database connection

CommandResponse handle_get(const char *key) {
    CommandResponse response = {.success = false, .exit = false};
    const char *select = "SELECT value FROM key_value_store WHERE key = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, select, -1, &stmt, NULL) != SQLITE_OK) {
        strcpy(response.error, "Error preparing database statement.\n");
    } else {
        sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            snprintf(response.data, MAX_RESPONSE_SIZE, "%s\n", sqlite3_column_text(stmt, 0));
            response.success = true;
        } else {
            strcpy(response.error, "Key not found.\n");
        }
        sqlite3_finalize(stmt);
    }

    return response;
}
