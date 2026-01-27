#include "helpers.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define REBROADCAST_DELAY 3

Destination hosts[MAX_CONNECTIONS] = {0};
int available_hosts = 0;
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
void recurring_broadcast(int sock, struct sockaddr_in broadcast_addr, M_TYPE mtype, size_t len, void *payload);

int udp_sock;
int tcp_sock;

static time_t start_time;

Destination *connection_exists(M_HEADER header)
{
      pthread_mutex_lock(&mux);
      for (int i = 0; i < MAX_CONNECTIONS; i++)
      {
            if(strcmp(header.hostname, hosts[i].hostname) == 0)
            {

                  pthread_mutex_unlock(&mux);
                  // printf("Connection already exists");
                  return &hosts[i];
            }
      }
      pthread_mutex_unlock(&mux);
      return NULL;
}


Destination *parse_identifying_message(void *buf)
{
      M_HEADER header;
      memcpy(&header, buf, sizeof(M_HEADER));
      
      // printf("Header parsed: MSG_TYPE: %d, IP FROM: %s, PORT FROM: %d, len: %ld\n", header.type, inet_ntoa(header.from_ip), ntohs(header.from_port), header.len);
      
      struct in_addr *my_ip = get_my_ip();

      Destination *host = connection_exists(header);
      if (header.from_ip.s_addr == my_ip->s_addr || host != NULL || header.type == M_LEAVING)
      {
            // printf("Ignoring!\n");
            free(my_ip);
            return host;
      }

      pthread_mutex_lock(&mux);

      Destination *ret = &hosts[available_hosts];
      strncpy(hosts[available_hosts].hostname, header.hostname, MAX_HOSTNAME_LEN);

      hosts[available_hosts].fd = NEW_HOST;
      hosts[available_hosts].host_port = header.from_port;
      hosts[available_hosts].ip_addr.s_addr = header.from_ip.s_addr;

      available_hosts++;

      pthread_mutex_unlock(&mux);


      raise(NEW_CONNECTION);
      free(my_ip);

      return ret;
}

struct in_addr *get_my_ip()
{
      char my_hostname[32];
      if (gethostname(my_hostname, 32) < 0)
      {
            perror("gethostname error\n");
            exit(EXIT_FAILURE);
      }

      struct hostent *host_entry = gethostbyname(my_hostname);
      char *my_ip = inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]));

      struct in_addr *res = malloc(sizeof(struct in_addr));
      *res = *((struct in_addr *) host_entry->h_addr_list[0]);
      return res;
}

char *get_my_hostname()
{
      char *my_hostname = calloc(MAX_HOSTNAME_LEN, sizeof(char));
      if (gethostname(my_hostname, MAX_HOSTNAME_LEN) < 0)
      {
            perror("gethostname error\n");
            exit(EXIT_FAILURE);
      }
      // printf("My hostname is: %s\n", my_hostname);
      return my_hostname;
}

void send_packet(int bsock, struct sockaddr_in sock_addr, M_TYPE type, size_t len, void *payload)
{
      struct in_addr *my_ip = get_my_ip();
      
      M_HEADER header = {0};
      header.type = type;
      header.from_ip = *my_ip;

      char *my_name = get_my_hostname();
      strncpy(header.hostname, my_name, strlen(my_name));

      header.from_port = htons(UDP_PORT);
      header.len = htonl(len);
      free(my_ip);
      
      uint8_t buf[sizeof(M_HEADER) + len];

      memcpy(buf, &header, sizeof(M_HEADER));

      if (payload != NULL)
      {
            memcpy(&buf[sizeof(M_HEADER)], payload, len);
      }

      if (sendto(bsock, buf, sizeof(M_HEADER) + len, 0, (const struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
      {
            perror("Client could not send to server\n");
            exit(EXIT_FAILURE);
      }
}

void recurring_broadcast(int sock, struct sockaddr_in broadcast_addr, M_TYPE mtype, size_t len, void *payload)
{
      time_t end_time = time(NULL);
      // printf("difftime = %f\n", difftime(end_time, start_time));

      if (difftime(end_time, start_time) <= REBROADCAST_DELAY)
      {
            return;
      }

      printf("Sending broadcast again\n");
      send_packet(sock, broadcast_addr, M_BROADCAST, 0, NULL);
      time(&start_time);
}

void remove_host(Destination *host)
{
      pthread_mutex_lock(&mux);
      memset(host, 0, sizeof(host)); 
      host->fd = EMPTY_HOST;
      available_hosts--;
      pthread_mutex_unlock(&mux);
      raise(DISCONNECT);
}

int main(int argc, char *argv[])
{
      // printf("Usage: ./my_airdrop [options] -p [port number]\n");
      // printf("Options are: \n");
      // printf("\t-h : Display help menu\n");
      // printf("\t-d : Debug printout - used for debugging\n");
      // printf("\t-p : Specify port to connect to (only used for debugging)\n");

      printf("My IP address: %s\n", inet_ntoa(*((struct in_addr *)get_my_ip())));
      // printf("sig == NEW_CONNECTION ? %d, sig == DISCONNECT ? %d \n", sig == NEW_CONNECTION, sig == DISCONNECT);
      printf("SIGUSR1 = %d, SIGUSR2 = %d, SIG NEW_CONNECTION = %d, SIG DISCONNECT = %d\n", SIGUSR1, SIGUSR2, NEW_CONNECTION, DISCONNECT);

      signal(SIGUSR1, signal_handler);
      signal(SIGUSR2, signal_handler);

      pthread_mutex_init(&mux, NULL);
      available_hosts = 0;
      connection_status_change = -1;

      for (int i = 0; i < MAX_CONNECTIONS; i++)
      {
            hosts[i].fd = EMPTY_HOST;
      }
      // Socket setup
      // bsock stands for broadcast socket - it is used to
      // send and receive all broadcast packets

      // Set up UDP socket
      udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

      if (udp_sock == -1)
      {
            perror("Socket error\n");
            exit(EXIT_FAILURE);
      }

      struct sockaddr_in sockaddr = {0};
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_port = htons(UDP_PORT);
      sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(udp_sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
      {
            perror("Socket bind failed\n");
            exit(EXIT_FAILURE);
      }
      
      int broadcastEnable = 1;

      if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
      {
            perror("Could not set sock option\n");
            exit(EXIT_FAILURE);
      }

      int flags = fcntl(udp_sock, F_GETFL, 0);
      if (flags == -1)
      {
            perror("fcntl failed\n");
            exit(EXIT_FAILURE);
      }

      if (fcntl(udp_sock, F_SETFL, flags | O_NONBLOCK) < 0)
      {
            perror("fcntl failed, udp_sock failed\n");
            exit(EXIT_FAILURE);
      }     

      struct sockaddr_in baddr = {0};
      baddr.sin_family = AF_INET;
      baddr.sin_addr.s_addr = INADDR_BROADCAST;
      baddr.sin_port = htons(UDP_PORT);
      // UDP socket set up

      // Set up TCP socket
      tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
      if (tcp_sock == -1)
      {
            perror("TCP Socket error\n");
            exit(EXIT_FAILURE);
      }

      struct sockaddr_in tcp_addr = {0};
      tcp_addr.sin_family = AF_INET;
      tcp_addr.sin_port = htons(TCP_PORT);
      tcp_addr.sin_addr.s_addr = INADDR_ANY;
      if (bind(tcp_sock, (struct sockaddr *) &tcp_addr, sizeof(tcp_addr)) < 0)
      {
            perror("TCP bind error\n");
            exit(EXIT_FAILURE);
      }

      if (listen(tcp_sock, MAX_CONNECTIONS) < 0)
      {
            perror("TCP Listen Failed\n");
            exit(EXIT_FAILURE);
      }
      // TCP socket set up

      // send broadcast packet
      // printf("Sending first broadcast packet\n");
      send_packet(udp_sock, baddr, M_BROADCAST, 0, NULL);
      time(&start_time);
      // start Sender Thread
      
      pthread_t sender;
      if (pthread_create(&sender, NULL, sender_function, NULL) < 0)
      {
            perror("Sender thread failed\n");
            exit(EXIT_FAILURE);
      }


      // Await broadcast notification
      while(1)
      {
            uint8_t buf[MAX_MSG_LEN];
      
            if (recv(udp_sock, buf, MAX_MSG_LEN, 0) < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                  recurring_broadcast(udp_sock, baddr, M_BROADCAST, 0, NULL);
                  continue;
            }
         
            Destination *host = parse_identifying_message(buf);
            switch (parse_mtype(buf))
            {    
                  case M_LEAVING:
                        remove_host(host);
                        
                  case M_ACK:
                        // printf("ack\n");
                        break;
                        
                  default:
                        break;
            }
      }

      return EXIT_SUCCESS;
}