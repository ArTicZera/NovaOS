#ifndef WAYLAND_COMPOSITOR_H
#define WAYLAND_COMPOSITOR_H

#include "protocol.h"
#include "surface.h"

#define MOUSE_LEFT   0
#define MOUSE_RIGHT  1
#define MOUSE_MIDDLE 2

void AddDirtyRect(int x, int y, int width, int height);
int RectsOverlap(DirtyRect *a, DirtyRect *b);
void MergeDirtyRect(DirtyRect *a, DirtyRect *b);
void RenderDirtyRects(void);
int RectsOverlapSurface(DirtyRect *rect, WLSurface *surface);
void DrawSurfaceClipped(WLSurface *surface, DirtyRect *clip);

void HandleMessage(int client, WLMessage *msg);

void WLCompositorUpdate();

void WLPointerMotion(int x, int y);

void WLPointerButton(int button, int pressed, int mouseX, int mouseY);

void DrawSurface(WLSurface* s);

void SaveSurfaceBackground(WLSurface *surface);
void RestoreSurfaceBackground(WLSurface *surface);
//void SaveBackgroundRect(int x, int y, int w, int h);
//void RestoreBackgroundRect(int x, int y, int w, int h);
void WLDrawRect(DWORD* buffer, int width, int height, int x, int y, int w, int h, DWORD color);

//void RoundRect(int x, int y, int w, int h, int radius, DWORD color);
void RoundRect(DWORD *buffer, int width, int height, int x, int y, int w, int h, int radius, DWORD color);


void SendEvent(int client, DWORD type, void* data, DWORD size);

WLWindow* WLCreateWindow(int x, int y, int w, int h, char* title);

void SendCreateSurface(WLWindow *win);
void SendAttachBuffer(WLWindow *win);
void CommitSurface(WLWindow *win);

#endif