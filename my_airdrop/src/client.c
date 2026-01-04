#include <helpers.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>


#define MAX_INPUT 256

void print_hosts()
{
      if (connection_status_change == NEW_CONNECTION)
      {
            printf("New Connection\n");
      }
      else if (connection_status_change == DISCONNECT)
      {
            printf("Old connection\n");
      }

      pthread_mutex_lock(&mux);
      for(int i = 0; i < available_hosts; i++)
      {
            printf("%d) %s\n", i+1, hosts[i].hostname);
      }
      pthread_mutex_unlock(&mux);
}

void graceful_shutdown(int code)
{
      struct sockaddr_in baddr = {0};
      baddr.sin_family = AF_INET;
      baddr.sin_addr.s_addr = INADDR_BROADCAST;
      baddr.sin_port = htons(UDP_PORT);

      send_packet(udp_sock, baddr, M_LEAVING, 0, NULL);
      close(udp_sock);
      close(tcp_sock);
      exit(code);
}

void *sender_function(void *vargs)
{
      char user_input[MAX_INPUT] = {0};

      signal(SIGINT, SIG_IGN);

      int flags = fcntl(fileno(stdin), F_GETFL, 0);
      flags |= O_NONBLOCK;

      if (fcntl(fileno(stdin), F_SETFL, flags) == -1)
      {
            perror("could not set stdin to be nonblocking\n");
            exit(EXIT_FAILURE);
      }

      printf("Usage: \n");
      printf("To list available hosts, do 'l'\n");
      printf("To select a host, type in its number\n");
      printf("To quit, type q\n");

      while (1)
      {
            if (fgets(user_input, MAX_INPUT, stdin) != NULL)
            {
                  if (strncmp(user_input, "l", 1) == 0)
                  {
                        printf("Printing available hosts...\n");
                        print_hosts();
                  }
                  else if (strncmp(user_input, "q", 1) == 0)
                  {
                        printf("Quitting...\n");
                        graceful_shutdown(EXIT_SUCCESS);
                  }
                  else
                  {
                        printf("Digit or unknown option. Yet to do\n");
                        continue;
                  }
            }
            if (connection_status_change != -1)
            {
                  print_hosts();
                  connection_status_change = -1;
            }
      }

      return NULL;
}