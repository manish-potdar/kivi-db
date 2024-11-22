#ifndef CONNECT_H
#define CONNECT_H

#include "config.h"

#define MAX_NODES 10

// Global array to store socket fds for peer connections
extern int peer_sockets[MAX_NODES];

// Function declarations
void connect_to_peers(const Config *config);
void cleanup_connections();

#endif
