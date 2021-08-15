#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

#define DEFAULT_NUM_PROCS 10

namespace NCursesDisplay {
void Display(System& system, int n = DEFAULT_NUM_PROCS);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(const float percent);
};  // namespace NCursesDisplay

#endif