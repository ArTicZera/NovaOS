#ifndef TIMER_H
#define TIMER_H

void PITIRQ0();
void Sleep(int seconds);
void InitTimer();
unsigned int GetTicks();

void RTCReadTime(int *hour, int *minute, int *second);
void RTCIRQHandler();

#endif
