#define WSCREEN 800
#define HSCREEN 600

void InitGraphics(LPDWORD mbinfo);
DWORD GetFramebuffer();
DWORD GetPitch();
void SetPixel(int x, int y, DWORD color);
DWORD GetPixel(int x, int y);
void ClearScreen(void);

void DrawRect(int x, int y, int w, int h, DWORD color);
