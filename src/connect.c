#include "../include/connect.h"
#include "../include/log.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global array for peer socket file descriptors
int peer_sockets[MAX_NODES] = {0};
PeerConnection peer_socket_connections[MAX_NODES] = {{0, 0} };

// Function to get the IP address of the current node
void get_self_ip(char *self_ip, size_t size) {
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Create a temporary socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Connect to a public address (this doesn't actually send data)
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80); // Port 80 (HTTP)
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr); // Google's DNS

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Connect failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Get the local IP address
    if (getsockname(sock, (struct sockaddr *)&addr, &addr_len) == -1) {
        perror("getsockname failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    strncpy(self_ip, inet_ntoa(addr.sin_addr), size);

    // Cleanup
    close(sock);
}

void connect_to_peers(const Config *config) {
  char self_ip[INET_ADDRSTRLEN];
  get_self_ip(self_ip, sizeof(self_ip));
  log_message("Node IP: %s\n", self_ip);

  for (int i = 0; i < config->num_nodes; i++) {
    // Skip connection to self
    if (strcmp(self_ip, config->node_ips[i]) == 0) {
      log_message("Skipping self-connection for IP: %s\n", self_ip);
      continue;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("Socket creation failed");
      continue;
    }

    struct sockaddr_in peer_addr;
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(config->port);

    if (inet_pton(AF_INET, config->node_ips[i], &peer_addr.sin_addr) <= 0) {
      perror("Invalid IP address");
      close(sock);
      continue;
    }

    log_message("Connectng to %s\n", config->node_ips[i]);
    if (connect(sock, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) == 0) {
      log_message("Connected to peer: %s\n", config->node_ips[i]);
      peer_sockets[i] = sock; // Save the socket fd
      peer_socket_connections[i].peer_socket_fd = sock;
      strcpy(peer_socket_connections[i].peer_ip_addr, config->node_ips[i]);
    } else {
      perror("Failed to connect to peer");
      close(sock);
    }
  }
}

void cleanup_connections() {
  for (int i = 0; i < MAX_NODES; i++) {
    if (peer_sockets[i] > 0) {
      close(peer_sockets[i]);
      peer_sockets[i] = 0;
    }
  }
}
