#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10
#define MAX_IP_LEN 16

typedef struct {
    int port;
    int max_connections;
    int num_nodes;
    char node_ips[MAX_NODES][MAX_IP_LEN];
} Config;

Config read_config(const char *config_file) {
    Config config = {0};
    FILE *file = fopen(config_file, "r");
    if (!file) {
        perror("Failed to open config file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "port=", 5) == 0) {
            config.port = atoi(line + 5);
        } else if (strncmp(line, "max_connections=", 16) == 0) {
            config.max_connections = atoi(line + 16);
        } else if (strncmp(line, "nodes=", 6) == 0) {
            char *nodes = line + 6;
            char *token = strtok(nodes, ",");
            while (token && config.num_nodes < MAX_NODES) {
                strncpy(config.node_ips[config.num_nodes++], token, MAX_IP_LEN - 1);
                token = strtok(NULL, ",");
            }
        }
    }

    fclose(file);
    return config;
}
