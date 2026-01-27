#ifndef HELPERS_H

#define HELPERS_H 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

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

#define NEW_CONNECTION SIGUSR1
#define DISCONNECT SIGUSR2

typedef struct 
{
      int fd;
      char hostname[MAX_HOSTNAME_LEN];
      in_port_t host_port;
      struct in_addr ip_addr;
} Destination;

enum {
      EMPTY_HOST,
      NEW_HOST,
};

typedef enum {
      M_BROADCAST,
      M_IDENTIFY,
      M_SEND,
      M_LEAVING,
      M_ACK,
      M_NACK,
} M_TYPE;

typedef struct
{
      M_TYPE type;
      struct in_addr from_ip;
      in_port_t from_port;
      char hostname[MAX_HOSTNAME_LEN];
      size_t len;
} M_HEADER;

char *mtype_to_s(M_TYPE m);

M_TYPE parse_mtype(uint8_t *buf);

Destination *parse_identifying_message(void *buf);

struct sockaddr_in *get_my_ip();

void send_packet(int bsock, struct sockaddr_in addr, M_TYPE type, size_t len, void *payload);

void *sender_function(void *vargs);

void signal_handler(int sig);



extern Destination hosts[MAX_CONNECTIONS];

extern int available_hosts;

extern pthread_mutex_t mux; 

extern sig_atomic_t connection_status_change;

extern int udp_sock;

extern int tcp_sock;

void graceful_shutdown(int code);

enum {
      BLUE_BLACK,
      WHITE_BLACK,
};

#endif