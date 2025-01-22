//VESA Video Memory
#define VIDMEM 0xFD000000

#define WSCREEN 640
#define HSCREEN 480

void SetPixel(int x, int y, BYTE color);
BYTE GetPixel(int x, int y);
void ClearScreen(void);

void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void DrawRect(int x, int y, int w, int h, BYTE color);
