#include "command_handler.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

extern sqlite3 *db; // Declare external database connection

CommandResponse handle_delete(const char *key) {
    CommandResponse response = {.success = false, .exit = false};
    const char *delete_query = "DELETE FROM key_value_store WHERE key = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, delete_query, -1, &stmt, NULL) != SQLITE_OK) {
        strcpy(response.error, "Error preparing database statement.\n");
    } else {
        sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            if (sqlite3_changes(db) > 0) {
                snprintf(response.data, MAX_RESPONSE_SIZE, "DELETED %s\r\n", key);
                response.success = true;
            } else {
                strcpy(response.error, "Error: Key not found.\r\n");
            }
        } else {
            strcpy(response.error, "Internal Error\r\n");
        }
        sqlite3_finalize(stmt);
    }

    return response;
}
