#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_MSG_LEN 1024

static int PORT = 8000;

int parse_args(int argc, char *argv[])
{
      int opt;
      int debug = 0;
      while ((opt = getopt(argc, argv, "hdp:")) != -1)
      {
            switch (opt)
            {
                  case 'h':
                        printf("Usage: ./my_airdrop [options] -p [port number]\n");
                        printf("Options are: \n");
                        printf("\t-h : Display help menu\n");
                        printf("\t-d : Debug printout - used for debugging\n");
                        printf("\t-p : Specify port to connect to (only used for debugging)\n");
                        break;
                        return -1;
                  case 'p':
                        PORT = atoi(optarg);
                        printf("Port specified: %d\n", PORT);
                        break;
                  default:
                        printf("Usage: ./my_airdrop [options] -p [port number]\n");
                        printf("Options are: \n");
                        printf("\t-h : Display help menu\n");
                        printf("\t-d : Debug printout - used for debugging\n");
                        printf("\t-p : Specify port to connect to (only used for debugging)\n");
                        return 0;
            }
      }
      return 1;
}

int main(int argc, char *argv[])
{
      if (!parse_args(argc, argv))
      {
            printf("Failed to parse args. Exiting\n");
            return EXIT_SUCCESS;
      }

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
            char buf[MAX_MSG_LEN];
            recvfrom(sockfd, buf, MAX_MSG_LEN, 0, NULL, NULL);
            printf("Recieved message: %s\n", buf);
      }

      return EXIT_SUCCESS;
}