//VESA Video Memory
#define VIDMEM 0xFD000000

#define WSCREEN 800
#define HSCREEN 600

void SetPixel(int x, int y, BYTE color);
BYTE GetPixel(int x, int y);
void ClearScreen(void);

void DrawRect(int x, int y, int w, int h, BYTE color);
