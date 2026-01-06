#include <helpers.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include "ncurses.h"
#include <time.h>

#define MAX_INPUT 256

static int length, cols, rows;

void print_hosts()
{
      uint8_t empty_host[sizeof(Destination)] = {0};
      
      pthread_mutex_lock(&mux);

      mvprintw(1, cols / 10, "Available Hosts %d", available_hosts);

      for(int i = 0; i < MAX_CONNECTIONS; i++)
      {
            if (hosts[i].fd != EMPTY_HOST)
            {
                  mvprintw(3 + i, cols / 10, "%d) %s\n", i+1, hosts[i].hostname);
            }
      }
      pthread_mutex_unlock(&mux);

      refresh();
}

// solution to loading animation is to make the 
// second line of the terminal only managed by the animation thread
void *animation(void *arg)
{
      curs_set(0);
      int x_pos = cols / 2;
      int y_pos = rows / 2; 

      char frames[] = {'/', '-', '\\', '|'};
      int num_chars = 4;

      mvprintw(1, cols / 2 - strlen("Searching"), "Searching");
      refresh();
      for (int frame = 0; frame >= 0; frame++)
      {
            mvaddch(1, cols / 2 - strlen("  Searching"), frames[frame % num_chars]);

            refresh();
            usleep(150000);
      }
}

void print_loading()
{
      pthread_t tid;
      pthread_create(&tid, NULL, animation, NULL);
}

void print_heading()
{
      char text[] = "Operating System-Independent File Sharing - OSIFS\n";
      print_loading();
      int center_x = (cols - strlen(text)) / 2;
      mvprintw(0, center_x, "%s", text);
      refresh();
}

void *sender_function(void *vargs)
{
      setlocale(LC_ALL, "");

      
      WINDOW *window = initscr();
      if (has_colors() == FALSE)
      { 
            endwin();
            printf("No colors supported\n");
            graceful_shutdown(EXIT_FAILURE);
      }
      
      start_color();
      cbreak();
      noecho();
      intrflush(stdscr, FALSE);
      keypad(stdscr, TRUE);
      
      getmaxyx(window, rows, cols);

      print_heading();  

      while (1)
      {
            if (connection_status_change != -1)
            {
                  print_hosts();
                  connection_status_change = -1;
            }
      }

      endwin();



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