#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <netdb.h>

#define PROTOCOL_PORT 8000

in_port_t port = 7999;

typedef struct {
      in_addr_t client_addr;
      uint16_t port;
} Connection;

typedef enum {
      M_BROADCAST,
      M_ESTABLISH,
      M_SEND,
} M_TYPE;

/*
Length refers to the length of the payload
*/
typedef struct
{
      M_TYPE type;
      struct in_addr from_ip;
      in_port_t from_port;
      size_t len;
} M_HEADER;

struct in_addr *get_my_ip();
void send_packet(int sockfd, struct sockaddr_in sockaddr, M_TYPE type, size_t len, void *payload);

/*
      Send_packet assumes everything is sent to it in host order
*/
void send_packet(int sockfd, struct sockaddr_in sock_addr, M_TYPE type, size_t len, void *payload)
{
      struct in_addr *my_ip = get_my_ip();
      
      M_HEADER header;
      header.type = type;
      header.from_ip = *my_ip;
      header.from_port = htons(port);
      header.len = htonl(len);
      
      uint8_t buf[sizeof(M_HEADER) + len];

      memcpy(buf, &header, sizeof(M_HEADER));

      if (payload != NULL)
      {
            memcpy(&buf[sizeof(M_HEADER)], payload, len);
      }
      printf("Sending buf: \n");
      for (int i = 0; i < sizeof(M_HEADER) + len; i++)
      {
            printf("%x", buf[i]);
      }

      if (sendto(sockfd, buf, sizeof(M_HEADER) + len, 0, (const struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
      {
            perror("Client could not send to server\n");
            exit(EXIT_FAILURE);
      }
}

struct in_addr *get_my_ip()
{
      char my_hostname[32];
      if (gethostname(my_hostname, 32) < 0)
      {
            perror("gethostname error\n");
            exit(EXIT_FAILURE);
      }

      printf("My hostname is: %s\n", my_hostname);

      struct hostent *host_entry = gethostbyname(my_hostname);
      char *my_ip = inet_ntoa(*((struct in_addr *) host_entry->h_addr));
      printf("My IP is: %s\n", my_ip);

      struct in_addr *res = malloc(sizeof(struct in_addr));
      *res = *((struct in_addr *) host_entry->h_addr);
      return res;
}

int main(int argc, char *argv[])
{
      printf("Attempting to bind to port %d\n", port);

      int discovery_sock = socket(AF_INET, SOCK_DGRAM, 0);
      
      if (discovery_sock == -1)
      {
            perror("Client socket failed\n");
            exit(EXIT_FAILURE);
      }

      int broadcastEnable = 1;

      if (setsockopt(discovery_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
      {
            perror("Could not set sock option\n");
            exit(EXIT_FAILURE);
      }

      struct sockaddr_in sock_addr = {0};
      sock_addr.sin_family = AF_INET;
      sock_addr.sin_port = htons(PROTOCOL_PORT);
      sock_addr.sin_addr.s_addr = INADDR_BROADCAST;
      // set up socket

      send_packet(discovery_sock, sock_addr, M_BROADCAST, 0, NULL);
      printf("Broadcast packet sent!\n");

      // now wait to hear back
      while (1)
      {
            
      }
}