void DrawChar(BYTE* bitmap, DWORD color);
void Print(const char* str, DWORD color);
void Debug(const CHAR* str, int debug);
void IntToString(int value, char* buffer);
void PrintInt(int value, DWORD color);
void PrintHex(int value, DWORD color);
void PrintOut(char letter, DWORD color);

int atoi(const char* str);
int GetCursorX();
int GetCursorY();
void SetCursorX(int x);
void SetCursorY(int y);
