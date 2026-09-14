#ifndef WAYLAND_PROTOCOL_H
#define WAYLAND_PROTOCOL_H

#include "../Include/stdint.h"

#include "surface.h"

#define WL_MAX_PAYLOAD 256
#define WL_SURFACE_CREATED 10

/* Mensagens */

typedef enum
{
    WL_NONE = 0,

    WL_CREATE_SURFACE,
    WL_DESTROY_SURFACE,
    WL_MOVE_SURFACE,
    WL_RESIZE_SURFACE,
    WL_ATTACH_BUFFER,
    WL_COMMIT,

    WL_SET_TITLE,
    WL_SET_POSITION,

    WL_POINTER_MOTION,
    WL_POINTER_BUTTON,

    WL_KEYBOARD_KEY,

    WL_REDRAW,
    WL_CLOSE,
    WL_FOCUS,
    WL_UNFOCUS,

} WLMessageType;

typedef struct
{
    DWORD type;
    DWORD size;
} WLHeader;

typedef struct
{
    WLHeader header;
    BYTE payload[WL_MAX_PAYLOAD];
} WLMessage;

typedef struct
{
    DWORD client;
    DWORD id;

    int x;
    int y;

    int width;
    int height;
} WLCreateSurface;

typedef struct
{
    DWORD id;
    int x;
    int y;
    int w;
    int h;
} WLCreateSurfaceRequest;

typedef struct
{
    DWORD surface;

    DWORD buffer;

    DWORD width;
    DWORD height;
} WLAttachBuffer;

typedef struct
{
    DWORD surface;
} WLCommit;

typedef struct
{
    DWORD surface;
} WLCommitRequest;


typedef struct
{
    DWORD surface;
    int x;
    int y;
} WLMoveSurfaceRequest;

typedef struct
{
    DWORD surface;
    int width;
    int height;
} WLResizeSurfaceRequest;

typedef struct
{
    DWORD surface;
    int x;
    int y;
} WLPointerMotionEvent;

typedef struct
{
    DWORD surface;
    int button;
    int pressed;

} WLPointerButtonEvent;

typedef struct
{
    DWORD surface;
    BYTE key;
    BYTE pressed;
} WLKeyboard;

typedef struct
{
    DWORD surface;
    char title[64];
} WLTitle;

typedef struct
{
    DWORD surface;
} WLFocusEvent;

typedef struct
{
    int socket;

    int id;

    DWORD *buffer;
    DWORD *backbuffer;

    int w, h;
    int x, y;

    char title[64];

    WLSurface* surface;

    int curX;
    int curY;
} WLWindow;

#endif