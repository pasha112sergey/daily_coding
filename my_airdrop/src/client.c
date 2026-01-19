#include <helpers.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include "ncurses.h"
#include <time.h>
#include "tui.h"

#define MAX_INPUT 256

static int length, cols, rows;



void *sender_function(void *vargs)
{

      pthread_t tui_thread;

      pthread_create(&tui_thread, NULL, tui, NULL);

      
      // getmaxyx(window, rows, cols);

      // print_heading();  







      // char user_input[MAX_INPUT] = {0};

      // signal(SIGINT, SIG_IGN);

      // int flags = fcntl(fileno(stdin), F_GETFL, 0);
      // flags |= O_NONBLOCK;

      // if (fcntl(fileno(stdin), F_SETFL, flags) == -1)
      // {
      //       perror("could not set stdin to be nonblocking\n");
      //       exit(EXIT_FAILURE);
      // }

      // printf("Usage: \n");
      // printf("To list available hosts, do 'l'\n");
      // printf("To select a host, type in its number\n");
      // printf("To quit, type q\n");

      // while (1)
      // {
      //       if (fgets(user_input, MAX_INPUT, stdin) != NULL)
      //       {
      //             if (strncmp(user_input, "l", 1) == 0)
      //             {
      //                   printf("Printing available hosts...\n");
      //                   print_hosts();
      //             }
      //             else if (strncmp(user_input, "q", 1) == 0)
      //             {
      //                   printf("Quitting...\n");
      //                   graceful_shutdown(EXIT_SUCCESS);
      //             }
      //             else
      //             {
      //                   printf("Digit or unknown option. Yet to do\n");
      //                   int digit = atoi(user_input);
      //                   continue;
      //             }
      //       }
      //       if (connection_status_change != -1)
      //       {
      //             print_hosts();
      //             connection_status_change = -1;
      //       }
      // }

      return NULL;
}