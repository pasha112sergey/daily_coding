#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

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
      size_t len;
} M_HEADER;

int main(int argc, char *argv[])
{
      int port = atoi(argv[1]);
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
      sock_addr.sin_port = htons(port);
      sock_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

      
      // first send broadcast
      if (connect(discovery_sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
      {
            perror("Client connect failed\n");
            exit(EXIT_FAILURE);
      }
      
      
      M_HEADER header = {M_ESTABLISH, 0};

      if (sendto(discovery_sock, &header, sizeof(header), 0,  (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
      {
            perror("Client could not send to server\n");
            exit(EXIT_FAILURE);
      }

      // now wait to hear back
      while (1)
      {
            
      }
}