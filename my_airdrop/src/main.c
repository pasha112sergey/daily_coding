#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAX_MSG_LEN 1024
#define MAX_CONNECTIONS 32
#define DISCOVERY_PORT 8000
#define COMM_PORT 8001

typedef struct {
      struct in_addr client_addr;
      uint16_t client_port;
      int socket;
} Connection;

typedef enum {
      M_BROADCAST,
      M_IDENTIFY,
      M_SEND,
      M_ACK,
      M_NACK,
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
      
      printf("Header parsed: MSG_TYPE: %d, IP FROM: %s, PORT FROM: %d, len: %ld\n", header.type, inet_ntoa(header.from_ip), ntohs(header.from_port), header.len);
      
      Connection *conn = malloc(sizeof(Connection));
      conn->client_addr = header.from_ip;
      conn->client_port = header.from_port;

      return conn;
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

void send_packet(int sockfd, struct sockaddr_in sock_addr, M_TYPE type, size_t len, void *payload)
{
      struct in_addr *my_ip = get_my_ip();
      
      M_HEADER header;
      header.type = type;
      header.from_ip = *my_ip;
      header.from_port = htons(COMM_PORT);
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

M_TYPE parse_mtype(uint8_t *buf)
{
      M_HEADER header;
      memcpy(&header, buf, sizeof(M_HEADER));

      return header.type;
}

char *mtype_to_s(M_TYPE m)
{
      switch (m)
      {
            case M_ACK:
                  return "ACK";
            case M_BROADCAST:
                  return "BROADCAST";
            case M_IDENTIFY:
                  return "IDENTIFY";
            case M_SEND:
                  return "SEND";
            case M_NACK:
                  return "NACK";
            default:
                  return "UNKNOWN";
      }
}

int main(int argc, char *argv[])
{
      printf("Usage: ./my_airdrop [options] -p [port number]\n");
      printf("Options are: \n");
      printf("\t-h : Display help menu\n");
      printf("\t-d : Debug printout - used for debugging\n");
      printf("\t-p : Specify port to connect to (only used for debugging)\n");

      // Socket setup
      int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

      if (sockfd == -1)
      {
            perror("Socket error\n");
            exit(EXIT_FAILURE);
      }

      struct sockaddr_in sockaddr = {0};
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_port = htons(DISCOVERY_PORT);
      sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
      {
            perror("Socket bind failed\n");
            exit(EXIT_FAILURE);
      }

      
      // Await broadcast 
      while(1)
      {
            uint8_t buf[MAX_MSG_LEN];
            recv(sockfd, buf, MAX_MSG_LEN, 0);

            M_TYPE rcvd = parse_mtype(buf);
            printf("Message of type %s recieved \n", mtype_to_s(rcvd));

            switch (rcvd)
            {    
                  case M_BROADCAST:
                        printf("Broadcast message recieved: %s\n", buf);
                        Connection *conn = parse_broadcast(buf);
                        
                        struct sockaddr_in send_addr; 
                        memset(&send_addr, 0, sizeof(send_addr));

                        send_addr.sin_family = AF_INET;
                        send_addr.sin_addr = conn->client_addr;
                        send_addr.sin_port = conn->client_port;

                        send_packet(sockfd, send_addr, M_IDENTIFY, 0, NULL);
                        break;
                  case M_ACK:
                        printf("ack\n");
                        break;
                  default:
                        break;
            }
      }

      return EXIT_SUCCESS;
}