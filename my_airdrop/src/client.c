#include <helpers.h>
#include <pthread.h>
#include <signal.h>

#define MAX_INPUT 256

void print_hosts()
{
      pthread_mutex_lock(&mux);

      for(int i = 0; i < available_hosts; i++)
      {
            printf("%d) %s\n", i+1, hosts[i].hostname);
      }

      pthread_mutex_unlock(&mux);
}

void *sender_function(void *vargs)
{
      char user_input[MAX_INPUT] = {0};

      signal(SIGINT, SIG_IGN);

      printf("Usage: \n");
      printf("To list available hosts, do 'l'\n");
      printf("To select a host, type in its number\n");
      printf("To quit, type q\n");

      while (1)
      {
            if (fgets(user_input, MAX_INPUT, stdin) < 0)
            {
                  printf("Error processing input\n");
                  continue;
            }

            if (strcmp(user_input, "l\n") == 0)
            {
                  printf("Printing available hosts...\n");
                  print_hosts();
            }
            if (strcmp(user_input, "q\n") == 0)
            {
                  printf("Quitting...\n");
                  exit(EXIT_FAILURE);
                  // shutdown_clean();
            }
            else
            {
                  printf("Digit or unknown option. Yet to do\n");
                  continue;
            }
      }

      return NULL;
}