#ifndef SYNC_H
#define SYNC_H

#define SYNC_SET_COMMAND "SYNC_SET __key __valye \r\n"

typedef struct {
    int sync_success; // number of server which respond
    int sync_failures; // number of server failures
} SyncResponse;

SyncResponse sync_set(const char* key, const char* value);
SyncResponse sync_update(const char* key, const char* value);
SyncResponse sync_delete(const char* key);

#endif // SYNC_H
