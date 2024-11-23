#include "../include/sync.h"
#include "../include/log.h"
#include "../include/connect.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>

#define RESPONSE_BUFFER_SIZE 256


SyncResponse sync_set(const char* key, const char* value) {

    char command[512];

    snprintf(command, sizeof(command), "SYNC_SET %s %s \r\n", key, value);
    log_message("Running command %s\n", command);
    printf("Running command %s\n", command);

    SyncResponse response;
    response.sync_success = 0;
    response.sync_failures = 0;

    // loop through socket fds from connect.h
    for(int idx = 0; idx < MAX_NODES; idx++) {
        PeerConnection peer_connection = peer_socket_connections[idx];

        if(peer_connection.peer_socket_fd == 0) {
            // socket not created or socket conn dropped
            syslog(LOG_INFO, "Skipping fd %d\n", peer_connection.peer_socket_fd);
            continue;
        }

        // send this command
        if(send(peer_connection.peer_socket_fd, command, strlen(command), 0) < 0) {
            perror("Sync send failed\n");
            response.sync_failures++;
            continue;
        }

        // read response
        char buffer[RESPONSE_BUFFER_SIZE];
        ssize_t bytes_received = recv(peer_connection.peer_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            // null terminate response
            buffer[bytes_received] = '\0';
            response.sync_success++;
        } else {
            perror("Receive failed\n");
            syslog(LOG_INFO, "Receive failed from IP: %s\n", peer_connection.peer_ip_addr);
            response.sync_failures++;
        }

    }

    return response;
    
}

SyncResponse sync_update(const char* key, const char* value) {

    char command[512];

    snprintf(command, sizeof(command), "SYNC_UPDATE %s %s \r\n", key, value);
    log_message("Running command %s\n", command);
    printf("Running command %s\n", command);

    SyncResponse response;
    response.sync_success = 0;
    response.sync_failures = 0;

    // loop through socket fds from connect.h
    for(int idx = 0; idx < MAX_NODES; idx++) {
        PeerConnection peer_connection = peer_socket_connections[idx];

        if(peer_connection.peer_socket_fd == 0) {
            // socket not created or socket conn dropped
            syslog(LOG_INFO, "Skipping fd %d\n", peer_connection.peer_socket_fd);
            continue;
        }

        // send this command
        if(send(peer_connection.peer_socket_fd, command, strlen(command), 0) < 0) {
            perror("Sync update failed\n");
            response.sync_failures++;
            continue;
        }

        // read response
        char buffer[RESPONSE_BUFFER_SIZE];
        ssize_t bytes_received = recv(peer_connection.peer_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            // null terminate response
            buffer[bytes_received] = '\0';
            response.sync_success++;
        } else {
            perror("Receive failed\n");
            syslog(LOG_INFO, "Receive failed from IP: %s\n", peer_connection.peer_ip_addr);
            response.sync_failures++;
        }

    }

    return response;
    
}

SyncResponse sync_delete(const char* key) {

    char command[512];

    snprintf(command, sizeof(command), "SYNC_DELETE %s \r\n", key);
    log_message("Running command %s\n", command);
    printf("Running command %s\n", command);

    SyncResponse response;
    response.sync_success = 0;
    response.sync_failures = 0;

    // loop through socket fds from connect.h
    for(int idx = 0; idx < MAX_NODES; idx++) {
        PeerConnection peer_connection = peer_socket_connections[idx];

        if(peer_connection.peer_socket_fd == 0) {
            // socket not created or socket conn dropped
            syslog(LOG_INFO, "Skipping fd %d\n", peer_connection.peer_socket_fd);
            continue;
        }

        // send this command
        if(send(peer_connection.peer_socket_fd, command, strlen(command), 0) < 0) {
            perror("Sync send failed\n");
            response.sync_failures++;
            continue;
        }

        // read response
        char buffer[RESPONSE_BUFFER_SIZE];
        ssize_t bytes_received = recv(peer_connection.peer_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            // null terminate response
            buffer[bytes_received] = '\0';
            response.sync_success++;
        } else {
            perror("Receive failed\n");
            syslog(LOG_INFO, "Receive failed from IP: %s\n", peer_connection.peer_ip_addr);
            response.sync_failures++;
        }

    }

    return response;
    
}