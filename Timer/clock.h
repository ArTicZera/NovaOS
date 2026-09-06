#ifndef CLOCK_H
#define CLOCK_H

typedef struct
{
    double angleH;
    double angleM;
    double angleS;

    int hours;
    int minutes;
    int seconds;
} Clock;

extern int hour, minute, second;


void DrawClock(Clock *clk, WLWindow* win, int cx, int cy);
void ClockUpdate(Clock *clk);
void ClockIRQ();
void InitClock();
void DrawPointer(WLWindow* win, int cx, int cy, double angle, int size, DWORD color);
void ClockPointer(double angle, int size, int *x, int *y);
void DrawHourMarks(WLWindow* win, int cx, int cy, int radius, DWORD color);

#endif