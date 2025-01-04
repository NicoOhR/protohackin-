#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT "5566"

int server_init(void) {
  int addr_size, server_socket, info_return, sockfd, bind_return;
  struct addrinfo hints, *res, connecting_address;
  addr_size = sizeof connecting_address;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  server_socket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

  if (server_socket == -1) {
    printf("[-] socket creation error\n");
    exit(-1);
  }
  printf("[+] TCP server socket connection created\n");

  info_return = getaddrinfo(NULL, PORT, &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  bind_return = bind(sockfd, res->ai_addr, res->ai_addrlen);

  if (info_return < 0 || bind_return < 0) {
    printf("[-] could not create and bind to socket");
    exit(-1);
  }

  freeaddrinfo(res);
  listen(sockfd, 10);
  return sockfd;
}

void *echo(void *fd_ptr) {
  int socket_fd = *(int *)fd_ptr;
  free(fd_ptr);

  char c_buffer[1];
  int received;

  recv(socket_fd, c_buffer, 1, 0);
  printf("%c", c_buffer[0]);
  fflush(stdout);
  while ((received = recv(socket_fd, c_buffer, 1, 0)) > 0) {
    printf("%c", c_buffer[0]);
    fflush(stdout);
  }
  pthread_exit(NULL);
}

int main(void) {
  int server_socket, sockfd, new_fd;
  socklen_t addr_size;
  char c_buffer[1];
  struct addrinfo their_addr;
  int bytes_received;
  sockfd = server_init();

  while (1) {
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    int *fd_ptr = malloc(sizeof(int));
    *fd_ptr = new_fd;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, echo, fd_ptr);
    pthread_detach(thread_id);
    if (new_fd < 0) {
      printf("[-] connection failed");
      exit(-1);
    }
    printf("connection made \n");
  }
}
