#ifndef NPAD_H
#define NPAD_H  

void ShowInfo(const char* filename);
void StartNotepad(const char* filename);
void EscapeNotepad();
void CreateNotepadFile(LPBYTE file, LPBYTE buffer, int length);

#endif