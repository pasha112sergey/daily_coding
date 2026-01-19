#ifndef TUI
#define TUI

#define WINDOW_NUM 2

static WINDOW *windows[WINDOW_NUM];

void *tui(void *args);
static void print_status();
static void print_hosts();

#endif