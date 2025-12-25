#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdint.h>
#include <arpa/inet.h>


#define MAX_MSG_LEN 1024
#define MAX_CONNECTIONS 32
#define PORT 8000

typedef struct {
      in_addr_t client_addr;
      uint16_t port;
} Connection;

typedef enum {
      M_BROADCAST,
      M_ESTABLISH,
      M_SEND,
} M_TYPE;

typedef struct
{
      M_TYPE type;
      struct in_addr from_ip;
      in_port_t from_port;
      size_t len;
} M_HEADER;


Connection *parse_broadcast(void *buf)
{
      M_HEADER header;
      memcpy(&header, buf, sizeof(M_HEADER));
      
      // char *str = inet_ntop(AF_INET, &(temp), ip_addr, sizeof(header.from_ip));
      printf("Header parsed: MSG_TYPE: %d, IP FROM: %s, PORT FROM: %d, len: %ld\n", header.type, inet_ntoa(header.from_ip), ntohs(header.from_port), header.len);
      
      Connection *conn = malloc(sizeof(Connection));
      return NULL;
}

int main(int argc, char *argv[])
{
      printf("Usage: ./my_airdrop [options] -p [port number]\n");
      printf("Options are: \n");
      printf("\t-h : Display help menu\n");
      printf("\t-d : Debug printout - used for debugging\n");
      printf("\t-p : Specify port to connect to (only used for debugging)\n");

      int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

      if (sockfd == -1)
      {
            perror("Socket error\n");
            exit(EXIT_FAILURE);
      }

      struct sockaddr_in sockaddr = {0};
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_port = htons(PORT);
      sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
      {
            perror("Socket bind failed\n");
            exit(EXIT_FAILURE);
      }

      while (1)
      {
            uint8_t buf[MAX_MSG_LEN];
            recvfrom(sockfd, buf, MAX_MSG_LEN, 0, NULL, NULL);
            printf("message recieved: %s\n", buf);
            Connection *conn = parse_broadcast(buf);
      }

      return EXIT_SUCCESS;
}