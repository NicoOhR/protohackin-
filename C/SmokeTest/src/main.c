#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {
  char *ip = "127.0.0.1";
  int port = 5566;
  int server_socket, sockfd, new_fd, info_return, bind_return;
  socklen_t addr_size;
  char buffer[124];
  struct addrinfo hints, *res, their_addr;

  addr_size = sizeof their_addr;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    printf("[-] socket creation error\n");
    exit(-1);
  }
  printf("[+] TCP server socket connection created\n");
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  info_return = getaddrinfo(NULL, "5566", &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  bind_return = bind(sockfd, res->ai_addr, res->ai_addrlen);

  if (info_return < 0 || bind_return < 0) {
    printf("[-] could not create and bind to socket");
    exit(-1);
  }

  listen(sockfd, 10);

  while (1) {
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd < 0) {
      printf("[-] connection failed");
      exit(-1);
    }
    printf("connection made \n");

    recv(new_fd, buffer, 124, 0);
    send(new_fd, buffer, 124, 0);
  }
}
