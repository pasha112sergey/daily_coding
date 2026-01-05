#include <helpers.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include "ncurses.h"

#define MAX_INPUT 256

void print_hosts()
{
      if (connection_status_change == NEW_CONNECTION)
      {
            printw("New Connection\n");
      }
      else if (connection_status_change == DISCONNECT)
      {
            printw("Disconnected! \n");
      }

      uint8_t empty_host[sizeof(Destination)] = {0};
      
      pthread_mutex_lock(&mux);
      printw("Available hosts %d\n", available_hosts);

      for(int i = 0; i < MAX_CONNECTIONS; i++)
      {
            if (hosts[i].fd != EMPTY_HOST)
            {
                  printw("%d) %s\n", i+1, hosts[i].hostname);
            }
      }
      pthread_mutex_unlock(&mux);
      refresh();
}

void print_heading(WINDOW *win)
{
      int length, cols, rows;
      getmaxyx(win, rows, cols);
      char text[] = "Operating System-Independent File Sharing - OSIFS";

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

      init_pair(1, COLOR_RED, COLOR_BLACK);
      attron(COLOR_PAIR(1));
      print_heading(window);      
      getch();
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