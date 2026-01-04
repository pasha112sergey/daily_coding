#include "helpers.h"

Destination hosts[MAX_CONNECTIONS] = {0};
int available_hosts = 0;
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

Connection *parse_broadcast(void *buf)
{
      M_HEADER header;
      memcpy(&header, buf, sizeof(M_HEADER));
      
      printf("Header parsed: MSG_TYPE: %d, IP FROM: %s, PORT FROM: %d, len: %ld\n", header.type, inet_ntoa(header.from_ip), ntohs(header.from_port), header.len);
      
      struct in_addr *my_ip = get_my_ip();

      if (header.from_ip.s_addr == my_ip->s_addr)
      {
            printf("Received my own broadcast!\n");
            free(my_ip);
            return NULL;
      }

      Connection *conn = malloc(sizeof(Connection));
      conn->client_addr = header.from_ip;
      conn->client_port = header.from_port;
      conn->client_name_len = ntohl(header.name_len);
      strncpy(conn->client_name, header.hostname, conn->client_name_len);

      free(my_ip);

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
      printf("My hostname is: %s\n", my_hostname);
      return my_hostname;
}

void send_packet(int bsock, struct sockaddr_in sock_addr, M_TYPE type, size_t len, void *payload)
{
      struct in_addr *my_ip = get_my_ip();
      
      M_HEADER header;
      header.type = type;
      header.from_ip = *my_ip;
      char *my_name = get_my_hostname();
      strncpy(header.hostname, my_name, strlen(my_name));
      header.name_len = htons((short) strlen(my_name));
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

int main(int argc, char *argv[])
{
      printf("Usage: ./my_airdrop [options] -p [port number]\n");
      printf("Options are: \n");
      printf("\t-h : Display help menu\n");
      printf("\t-d : Debug printout - used for debugging\n");
      printf("\t-p : Specify port to connect to (only used for debugging)\n");

      pthread_mutex_init(&mux, NULL);
      available_hosts = 0;
      for (int i = 0; i < MAX_CONNECTIONS; ++i)
      {
            hosts[i].fd = 0;
            hosts[i].hostname = NULL;
            hosts[i].ip_addr.s_addr = 0;
      }
      // Socket setup
      // bsock stands for broadcast socket - it is used to
      // send and receive all broadcast packets

      // Set up UDP socket
      int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

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

      struct sockaddr_in baddr = {0};
      baddr.sin_family = AF_INET;
      baddr.sin_addr.s_addr = INADDR_BROADCAST;
      baddr.sin_port = htons(UDP_PORT);
      // UDP socket set up

      // Set up TCP socket
      int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
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
      send_packet(udp_sock, baddr, M_BROADCAST, 0, NULL);

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
            recv(udp_sock, buf, MAX_MSG_LEN, 0);

            printf("Message of type %s received \n", mtype_to_s(parse_mtype(buf)));
            
            switch (parse_mtype(buf))
            {    
                  case M_BROADCAST:
                        printf("Broadcast message received: %s\n", buf);
                        Connection *conn;
                        
                        if ((conn = parse_broadcast(buf)) == NULL)
                              continue;

                        struct sockaddr_in send_addr = {0};
                        send_addr.sin_family = AF_INET;
                        send_addr.sin_addr = conn->client_addr;
                        send_addr.sin_port = conn->client_port;

                        send_packet(udp_sock, send_addr, M_IDENTIFY, 0, NULL);

                        pthread_mutex_lock(&mux);

                        hosts[available_hosts].fd = -1;
                        // memset(hosts[available_hosts].hostname, 0, MAX_HOSTNAME_LEN);
                        hosts[available_hosts].hostname = conn->client_name;
                        hosts[available_hosts].ip_addr = conn->client_addr;

                        available_hosts++;
                        pthread_mutex_unlock(&mux);
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