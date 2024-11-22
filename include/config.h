#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define MAX_NODES 10
#define MAX_IP_LEN 16

typedef struct {
    int port;
    int max_connections;
    int num_nodes;
    char node_ips[MAX_NODES][MAX_IP_LEN];
} Config;

Config read_config(const char *config_file);

#endif
