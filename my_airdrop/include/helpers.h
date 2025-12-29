#ifndef HELPERS_H
#define HELPERS_H

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

#define MAX_MSG_LEN 1e+8
#define MAX_CONNECTIONS 32
#define DISCOVERY_PORT 8000

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

char *mtype_to_s(M_TYPE m);
M_TYPE parse_mtype(uint8_t *buf);

#endif