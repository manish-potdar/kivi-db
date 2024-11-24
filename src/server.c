#include "../include/command_handler.h"
#include "../include/config.h"
#include "../include/connect.h"
#include "../include/log.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>

#define CONFIG_FILE "server.conf"

// #define PORT 8090
#define BUFFER_SIZE 1024
#define BACKLOG_QUEUE_SIZE 100

int server_fd;

// locks
pthread_cond_t cond_var;
pthread_mutex_t global_lock;

typedef struct client_data {
  int socket_fd;
  struct sockaddr_in address;
} Task;

int server_fd = -1;

// task queue
#define TASK_QUEUE_SIZE 100
Task task_queue[TASK_QUEUE_SIZE];
int front = 0;
int rear = 0;
int count = 0;

bool is_empty() { return count == 0; }

bool is_full() { return count == TASK_QUEUE_SIZE; }

void enqueue(Task task) {
  pthread_mutex_lock(&global_lock);

  // wait till queue has space (if could also be used but not)
  while (is_full()) {
    pthread_cond_wait(&cond_var, &global_lock);
  }

  // add task to queue
  task_queue[rear] = task;
  rear = (rear + 1) % TASK_QUEUE_SIZE;
  count++;

  // signal worker threads that a new task has been added
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&global_lock);
}

// Dequeue a task
Task dequeue() {
  pthread_mutex_lock(&global_lock);

  // check if queue is empty (wait for task to be available)
  while (is_empty()) {
    pthread_cond_wait(&cond_var, &global_lock);
  }

  // get task from the queue
  Task task = task_queue[front];
  front = (front + 1) % TASK_QUEUE_SIZE;
  count--;

  // signal that there is space in the queue
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&global_lock);
  return task;
}

// worker thread to handle connected clients
void *worker(void *args) {

  // while (true) {

  // Task client = dequeue();
  Task *client = (Task *)args;

  int new_socket = client->socket_fd;
  // int new_socket = client.socket_fd;

  char buffer[BUFFER_SIZE] = {0};

  log_message("Worker assigned to socket %d\n", new_socket);
  char welcome_response[] = "Welcome to kivi db\r\n";

  // sending welcome response
  send(new_socket, welcome_response, strlen(welcome_response), 0);

  const char *request = "hello\r\n";
  const char *response = "world\r\n";

  while (1) {
    // clear buffer
    memset(buffer, 0, sizeof(buffer));

    // read client data
    int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    // close connection on nothing received
    log_message("bytes read: %d\n", bytes_read);
    if (bytes_read <= 0) {
      log_message("Client disconnected...\n");
      break;
    }

    buffer[bytes_read] = '\0';

    log_message("recv from client: %s\n", buffer);
    int len = strlen(buffer);
    log_message("rev length: %ld\n", strlen(buffer));
    log_message("last chars: %d %d\n", buffer[bytes_read - 1],
           buffer[bytes_read - 2]);

    CommandResponse command_response = parse_command(buffer);

    // handle success insert
    if (command_response.success) {

      log_message("\nKey-value pair set..\n");
      send(new_socket, command_response.data, strlen(command_response.data), 0);
    }

    // handle exit command
    if (command_response.exit) {
      log_message("\nClient exit request..\n");
      const char exit_response[4] = "bye\n";
      send(new_socket, command_response.error, strlen(command_response.error),
           0);
      break;
    }

    // handle invalid command
    if (!command_response.exit && !command_response.success) {
      log_message("Invalid command...\n");
      send(new_socket, command_response.error, sizeof(command_response.error),
           0);
    }
  }

  // closing client connection in loop`
  close(new_socket);
  log_message("connection closed in loop\n");
  // }

  return NULL;
}

void handle_sigint(int sig) {
  log_message("\nShutting down the server gracefully...\n");

  // Close the server socket
  if (server_fd > 0) {
    close(server_fd);
    log_message("Server socket closed.\n");
  }

  // Perform any additional cleanup here
  // e.g., closing logs, releasing resources

  // destroy locks
  pthread_cond_destroy(&cond_var);
  pthread_mutex_destroy(&global_lock);

  close_database();

  // Cleanup connections on exit
  cleanup_connections();
  log_message("Server shutting down...\n");

  // Close syslog connection
  closelog();

  exit(0); // Exit the program
}

int main(int argc, char const *argv[]) {

  // Register signal handler for SIGINT
  signal(SIGINT, handle_sigint);

  // if (argc != 3) {
  //   log_message("usage: ./server <port> <max_connections>\n");
  //   exit(1);
  // }
  Config config = read_config(CONFIG_FILE);

  // Open syslog connection
  openlog("my_program", LOG_PID | LOG_CONS, LOG_USER);

  // int port = atoi(argv[1]);
  int port = config.port;
  // int thread_pool_size = atoi(argv[2]);

  // init db
  if (initialize_database() != 0) {
    fprintf(stderr, "Failed to initialize database.\n");
    exit(1);
  }

  // create threads
  // pthread_t worker_threads[thread_pool_size];

  // for (int i = 0; i < thread_pool_size; i++) {
  //   if (pthread_create(worker_threads + i, NULL, worker, NULL)) {
  //     perror("thread pool creation error");
  //     exit(2);
  //   }
  // }

  // initialize locks
  pthread_cond_init(&cond_var, NULL);
  pthread_mutex_init(&global_lock, NULL);

  // create fds and socket addr
  int new_socket;
  struct sockaddr_in address;

  int addrlen = sizeof(address);

  // create socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  // socket reuse
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("Rerused faield\n");
    exit(1);
  }

  // binding socket to a port (8080)
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(1);
  }

  if (listen(server_fd, BACKLOG_QUEUE_SIZE) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(1);
  }

  log_message("server listening on port %d\n", port);
  log_message("server listening on port %d\n", port);

  // connect to peer nodes on server startup
  // connect_to_peers(&config);
  connect_to_peers_with_retry(&config);

  int count = 0;

  // accepting connections
  while (1) {

    Task *client = malloc(sizeof(Task));

    // accept client and setup connection
    if ((client->socket_fd =
             accept(server_fd, (struct sockaddr *)&client->address,
                    (socklen_t *)&addrlen)) < 0) {
      perror("accept failed");
      close(server_fd);
      exit(1);
    }
    log_message("new incoming client\n");

    // enqueue task to queue so that threads can pickup
    // enqueue(client);

    // create a new thread to handle client connection
    pthread_t client_thread;
    if (pthread_create(&client_thread, NULL, &worker, (void *)client) != 0) {
      perror("thread creation failed\n");
      free(client);
      close(server_fd);
      exit(1);
    }

    // detach thread so that it cleans up itself
    pthread_detach(client_thread);
  }

  // destroy locks
  pthread_cond_destroy(&cond_var);
  pthread_mutex_destroy(&global_lock);

  close_database();

  // Cleanup connections on exit
  cleanup_connections();
  log_message("Server shutting down...\n");

  // Close syslog connection
  closelog();

  return 0;
}
