#ifndef SHELL_H
#define SHELL_H

#include "../Userspace/GUI/win.h"

extern int shellNOGUI;
extern int winshellX;
extern int winshellY;

void StartShellNoGUI();
void StartShellGUI(WINDOW* win);
void PrintWelcomeMSG();
void PrintCurrentDir();
void SaveTerminalScreen();
void ProcessShellCMD(char* command, int x, int y);
void ProcessShellRun(char* proccess);
void OnWindowMoved(WINDOW *win);

#endif
