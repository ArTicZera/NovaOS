#define MAXITEMS 5

typedef struct 
{
    char name[16];
    int type;  // 0 = Folder, 1 = File
    int x, y;
} DesktopItem;

DesktopItem* GetClickedItem(int x, int y);
void SetDesktopIndex(char* name, int type, int x, int y);
void DesktopEvents(int x, int y, int pressed);