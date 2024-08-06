#include "server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4221
#define EXIT_FAILURE 1
#define LISTEN_BACKLOG 50
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void socket_addr_init(struct sockaddr_in *servaddr) {
  memset(servaddr, 0, sizeof(struct sockaddr_in));
  servaddr->sin_family = AF_INET;
  servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr->sin_port = htons(PORT);
};

int main() {
  int sockfd, connfd, ret, len;
  struct sockaddr_in servaddr, client;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    handle_error("Error Creating Socket");
  }

  socket_addr_init(&servaddr);
  ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (ret == -1) {
    handle_error("Error Binding to Server Address");
  }

  ret = listen(sockfd, LISTEN_BACKLOG);
  if (ret == -1) {
    handle_error("Error Listening to Port");
  }

  len = sizeof(client);
  connfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&(len));
  if (connfd == -1) {
    handle_error("Error Accepting Client");
  }
  printf("Client Connected!\n");

  char *reply = "HTTP/1.1 200 OK\r\n\r\n";
  int bytes_sent = send(connfd, reply, strlen(reply), 0);
  if (bytes_sent == -1) {
    handle_error("Error Sending Bytes");
  }

  close(connfd);
  close(sockfd);
}
