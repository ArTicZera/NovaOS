#ifndef TEXT_H
#define TEXT_H

#define BYTES_PER_ROW ((WFONT + 7) / 8)
#define GLYPH_SIZE (BYTES_PER_ROW * HFONT)

void DrawChar(BYTE* bitmap, DWORD color);
void Print(const char* str, DWORD color);
void Debug(const CHAR* str, int debug);
void IntToString(int value, char* buffer);
void PrintInt(int value, DWORD color);
void PrintHex(int value, DWORD color);
void PrintOut(char letter, DWORD color);
void PrintHex32(DWORD value);
void PrintByteHex(BYTE b);
int GetCursorX();
int GetCursorY();
void SetCursorX(int x);
void SetCursorY(int y);
void MapFont();

#endif
