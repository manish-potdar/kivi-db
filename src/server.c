#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/command_handler.h"

// #define PORT 8090
#define BUFFER_SIZE 1024
#define BACKLOG_QUEUE_SIZE 100

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

  while (true) {

    Task client = dequeue();

    int new_socket = client.socket_fd;

    char buffer[BUFFER_SIZE] = {0};

    printf("Worker assigned to socket %d\n", new_socket);
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
      buffer[bytes_read] = '\0';

      printf("recv from client: %s\n", buffer);
      int len = strlen(buffer);
      printf("rev length: %ld\n", strlen(buffer));
      printf("last chars: %d %d\n", buffer[bytes_read-1], buffer[bytes_read-2]);

      CommandResponse command_response = parse_command(buffer);

      // handle exit command
      if(command_response.exit) {
        printf("\nClient exit request..\n");
        const char exit_response[4] = "bye\n";
        send(new_socket, command_response.error, strlen(command_response.error), 0);
        break;
      }

      // handle invalid command
      if(!command_response.exit && !command_response.success) {
        printf("Invalid command...\n");
        send(new_socket, command_response.error, sizeof(command_response.error), 0);
      }

    }

    // closing client connection in loop`
    close(new_socket);
    printf("connection closed in loop\n");
  }

  return NULL;
}

int main(int argc, char const *argv[]) {

  if (argc != 3) {
    printf("usage: ./server <port> <thread_pool_size>\n");
    exit(1);
  }

  int port = atoi(argv[1]);
  int thread_pool_size = atoi(argv[2]);

  // create threads
  pthread_t worker_threads[thread_pool_size];

  for (int i = 0; i < thread_pool_size; i++) {
    if (pthread_create(worker_threads + i, NULL, worker, NULL)) {
      perror("thread pool creation error");
      exit(2);
    }
  }

  // initialize locks
  pthread_cond_init(&cond_var, NULL);
  pthread_mutex_init(&global_lock, NULL);

  // create fds and socket addr
  int server_fd, new_socket;
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

  printf("server listening on port %d\n", port);

  int count = 0;

  // accepting connections
  while (1) {

    Task client;

    // accept client and setup connection
    if ((client.socket_fd =
             accept(server_fd, (struct sockaddr *)&client.address,
                    (socklen_t *)&addrlen)) < 0) {
      perror("accept failed");
      close(server_fd);
      exit(1);
    }
    printf("new incoming client\n");

    // enqueue task to queue so that threads can pickup
    enqueue(client);
  }

  // destroy locks
  pthread_cond_destroy(&cond_var);
  pthread_mutex_destroy(&global_lock);

  return 0;
}
