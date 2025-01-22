void StartButtonHandler(int x, int y, int pressed);

void UserSpace();
void DrawBackground();
void DrawTaskbar();
void UpdateExplorer();
void UserspaceState(int state);
void DesktopIcons();
void DrawBootScr();

void SaveStartArea();
void RestoreStartArea();

void MaximizeWindow(int x, int y, int pressed);
void CloseWindow(int x, int y, int pressed);

void HandleWindowDragging(WINDOW* window, int mouseX, int mouseY, int mousePressed);

void GetWindow(WINDOW window);
