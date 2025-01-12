void DrawChar(BYTE* bitmap, BYTE color);
void Print(const char* str, BYTE color);
void Debug(const CHAR* str, int debug);
void IntToString(int value, char* buffer);
void PrintInt(int value, BYTE color);
void PrintHex(int value, BYTE color);
void PrintOut(char letter, BYTE color);

int atoi(const char* str);
int GetCursorX();
int GetCursorY();
void SetCursorX(int x);
void SetCursorY(int y);