#ifndef SHELL_H
#define SHELL_H

#include "../Userspace/GUI/win.h"
#include "../Wayland/compositor.h"

extern int shellNOGUI;
extern int winshellX;
extern int winshellY;

void StartShellNoGUI();
void StartShellGUI(WLWindow* win);
void PrintWelcomeMSG();
void PrintCurrentDir();
void ProcessShellCMD(char* command, int x, int y);
void ProcessShellRun(char* proccess);
#endif
