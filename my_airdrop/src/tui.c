#include <ncurses.h>
#include "tui.h"
#include "helpers.h"
#include <panel.h>

static int row, cols;
static WINDOW *win;
static WINDOW *windows[WINDOW_NUM];

static void print_status()
{
      pthread_mutex_lock(&mux);
      int status = connection_status_change;
      pthread_mutex_unlock(&mux);

      char *message = ".. See Hosts Below ..";
      mvprintw(1, (cols - strlen(message)) / 2, "%s", message);
}

static void print_hosts()
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


void *tui(void *)
{
      win = initscr();
      getmaxyx(win, row, cols);
      cbreak();
      noecho();

      PANEL *panels[WINDOW_NUM];

      windows[0] = newwin(row - 4, cols / 2, 2, 0);
      windows[1] = newwin(row - 4, cols / 2, 2, cols/2);

      refresh();


      mvwprintw(windows[0], 1, cols / 4 - strlen("Devices"), "Devices");
      box(windows[0], 0, 0);
      wrefresh(windows[0]);


      mvwprintw(windows[1], 1, cols/4 - strlen("Files"), "Files");
      box(windows[1], 0, 0);
      wrefresh(windows[1]);

      
      mvprintw(0, (cols - strlen("Send Files with OSIFS!")) / 2, "%s", "Send Files with OSIFS!\n");
      
      // while (getch())
      {
            print_status();
            if (connection_status_change != -1)
            {
                  // print_hosts();
                  connection_status_change = -1;
            }
            
            // for(int i = 0; i < WINDOW_NUM; i++)          
                  // wrefresh(windows[i]); 
            refresh();
      }

      getch();
      graceful_shutdown(EXIT_SUCCESS);
      return NULL;
}
