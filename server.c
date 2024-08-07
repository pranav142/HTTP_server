#include "server.h"
#include <netinet/in.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4221
#define EXIT_FAILURE 1
#define LISTEN_BACKLOG 50
#define BUFFER_SIZE 100
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void send_200_response(int *connfd) {
  char *reply = "HTTP/1.1 200 OK\r\n\r\n";
  int bytes_sent = send(*connfd, reply, strlen(reply), 0);
  if (bytes_sent == -1) {
    handle_error("Error Sending Bytes");
  }
}

void read_request(int *connfd) {
  int len;
  char *buffer = (char *)malloc(BUFFER_SIZE);
  ssize_t msg_size = recv(*connfd, buffer, BUFFER_SIZE, 0);
  if (msg_size == 0) {
    printf("No Message Recieved\n");
    return;
  } else if (msg_size == -1) {
    printf("Failed to Read Message\n");
    return;
  }

  //  regex_t regex;
  //  int value;
  //  value = regcomp(&regex, "/", 0);
  //  // buffer[msg_size + 1] = '\0';
  printf("%s", buffer);
}

void handle_client(int *connfd) {
  read_request(connfd);
  send_200_response(connfd);
}

void socket_addr_init(struct sockaddr_in *servaddr) {
  memset(servaddr, 0, sizeof(struct sockaddr_in));
  servaddr->sin_family = AF_INET;
  servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr->sin_port = htons(PORT);
}

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
  while (1) {
    connfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&(len));
    if (connfd == -1) {
      handle_error("Error Accepting Client");
    }
    printf("Client Connected!\n");
    handle_client(&connfd);
  }

  close(connfd);
  close(sockfd);
}
