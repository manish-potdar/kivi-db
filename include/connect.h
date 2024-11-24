#ifndef CONNECT_H
#define CONNECT_H

#include "config.h"

#define MAX_NODES 10

typedef struct {
    int peer_socket_fd;
    char peer_ip_addr[16];
} PeerConnection;

// Global array to store socket fds for peer connections
extern int peer_sockets[MAX_NODES];
extern PeerConnection peer_socket_connections[MAX_NODES];

// Function declarations
void connect_to_peers(const Config *config);
void connect_to_peers_with_retry(const Config *config);
void cleanup_connections();

#endif
