#ifndef HELPERS_H

#define HELPERS_H 0

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
#include <pthread.h>

#define MAX_MSG_LEN 1<<10
#define MAX_CONNECTIONS 32
#define UDP_PORT 8000
#define TCP_PORT 8001
#define MAX_HOSTNAME_LEN 32

typedef struct 
{
      int fd;
      char *hostname;
      struct in_addr ip_addr;
} Destination;

extern Destination hosts[MAX_CONNECTIONS];

extern int available_hosts;

extern pthread_mutex_t mux; 

typedef struct {
      struct in_addr client_addr;
      uint16_t client_port;
      char client_name[MAX_HOSTNAME_LEN];
      int client_name_len;
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
      char hostname[MAX_HOSTNAME_LEN];
      short name_len;
      size_t len;
} M_HEADER;

char *mtype_to_s(M_TYPE m);

M_TYPE parse_mtype(uint8_t *buf);

Connection *parse_broadcast(void *buf);

struct in_addr *get_my_ip();

void send_packet(int bsock, struct sockaddr_in addr, M_TYPE type, size_t len, void *payload);

void *sender_function(void *vargs);

#endif