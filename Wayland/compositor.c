#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../Memory/alloc.h"
#include "../Drivers/mouse.h"
#include "../Userspace/userspace.h"

#include "af_unix.h"
#include "compositor.h"
#include "surface.h"

extern WLSurface surfaces[WL_MAX_SURFACES];
static DWORD nextID = 1;

static WLSurface* dragSurface = NULL;
static int dragOffsetX;
static int dragOffsetY;

static DirtyRect dirtyRects[MAX_DIRTY_RECTS];
static int dirtyRectCount = 0;

void AddDirtyRect(int x, int y, int width, int height)
{
    DirtyRect newRect;

    newRect.x = x;
    newRect.y = y;
    newRect.w = width;
    newRect.h = height;

    for (int i = 0; i < dirtyRectCount; i++)
    {
        if (RectsOverlap(&dirtyRects[i], &newRect))
        {
            MergeDirtyRect(&dirtyRects[i], &newRect);
            return;
        }
    }

    if (dirtyRectCount < MAX_DIRTY_RECTS)
    {
        dirtyRects[dirtyRectCount] = newRect;
        dirtyRectCount++;
    }
}

int RectsOverlap(DirtyRect *a, DirtyRect *b)
{
    if (a->x + a->w < b->x) return 0;
    if (b->x + b->w < a->x) return 0;
    if (a->y + a->h < b->y) return 0;
    if (b->y + b->h < a->y) return 0;

    return 1;
}

void MergeDirtyRect(DirtyRect *a, DirtyRect *b)
{
    int x1 = a->x < b->x ? a->x : b->x;
    int y1 = a->y < b->y ? a->y : b->y;

    int x2 =
        (a->x + a->w) >
        (b->x + b->w)
        ? (a->x + a->w)
        : (b->x + b->w);

    int y2 =
        (a->y + a->h) >
        (b->y + b->h)
        ? (a->y + a->h)
        : (b->y + b->h);

    a->x = x1;
    a->y = y1;
    a->w = x2 - x1;
    a->h = y2 - y1;
}

void RenderDirtyRects(void)
{
    for (int i = 0; i < dirtyRectCount; i++)
    {
        DirtyRect *rect = &dirtyRects[i];

        for (int s = 0; s < WL_MAX_SURFACES; s++)
        {
            WLSurface *surface = &surfaces[s];

            if (!surface->used)
                continue;

            if (!RectsOverlapSurface(rect, surface))
                continue;

            DrawSurfaceClipped(surface, rect);
        }
    }

    dirtyRectCount = 0;
}

int RectsOverlapSurface(DirtyRect *rect, WLSurface *surface)
{
    if (surface->x + surface->width < rect->x) return 0;
    if (rect->x + rect->w < surface->x) return 0;
    if (surface->y + surface->height < rect->y) return 0;
    if (rect->y + rect->h < surface->y) return 0;

    return 1;
}

void DrawSurfaceClipped(WLSurface *surface, DirtyRect *clip)
{
    int xStart = surface->x;
    int yStart = surface->y;

    int xEnd = surface->x + surface->width;
    int yEnd = surface->y + surface->height;

    if (xStart < clip->x) xStart = clip->x;

    if (yStart < clip->y) yStart = clip->y;

    if (xEnd > clip->x + clip->w)
        xEnd = clip->x + clip->w;

    if (yEnd > clip->y + clip->h)
        yEnd = clip->y + clip->h;

    for (int y = yStart; y < yEnd; y++)
    {
        for (int x = xStart; x < xEnd; x++)
        {
            int surfaceX = x - surface->x;
            int surfaceY = y - surface->y;

            DWORD pixel = surface->buffer[surfaceY * surface->width + surfaceX];

            SetPixel(x, y, pixel);
        }
    }
}

void HandleMessage(int client, WLMessage *msg)
{
    //Print("CALLED!", 0xFF00FF00);

    switch(msg->header.type)
    {
        case WL_CREATE_SURFACE:
        {
            //Print(" WL_CREATE_SURFACE!", 0xFF00FF00);

            WLCreateSurfaceRequest* req;

            req = (WLCreateSurfaceRequest*)msg->payload;

            WLCreateSurfaceFunc(req->id, req->x, req->y, req->w, req->h);

            break;
        }
        case WL_DESTROY_SURFACE:
            break;

        case WL_ATTACH_BUFFER:
        {
            //Print(" WL_ATTACH_BUFFER_REQUEST!", 0xFF00FF00);

            WLAttachBufferRequest* req;

            req = (WLAttachBufferRequest*)msg->payload;

            WLSurface* surface;

            surface = WLFindSurface(req->surface);

            if (surface)
            {
                surface->buffer = (LPDWORD) req->buffer;

                surface->width = req->w;
                surface->height = req->h;

                //req->w req->h??????? PROBLEMA ENCONTRADO
                surface->background = AllocateMemory(WSCREEN * HSCREEN * sizeof(DWORD));

                //CommitSurface(win);

                /*
                if(backBuffer == NULL)
                {
                    backBuffer = AllocateMemory(req->w * req->h * sizeof(DWORD));
                    SaveBackgroundRect(surface->x, surface->y, surface->width, surface->height);
                }
                */
            }

            break;
        }
        case WL_COMMIT:
        {
            WLCommitRequest* req;

            req = (WLCommitRequest*)msg->payload;

            WLSurface* surface;

            surface = WLFindSurface(req->surface);

            if (surface)
            {
                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );
            }

            break;
        }
        case WL_MOVE_SURFACE:
        {
            WLMoveSurfaceRequest *req;

            req = (WLMoveSurfaceRequest*)msg->payload;

            WLSurface *surface;

            surface = WLFindSurface(req->surface);

            if(surface)
            {
                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );

                surface->x = req->x;
                surface->y = req->y;

                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );
            }

            break;
        }
        case WL_RESIZE_SURFACE:
        {
            WLResizeSurfaceRequest *req;

            req = (WLResizeSurfaceRequest*)msg->payload;

            WLSurface *surface;

            surface = WLFindSurface(req->surface);

            if(surface)
            {
                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );

                surface->width = req->width;
                surface->height = req->height;

                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );
            }

            break;
        }
        case WL_SET_POSITION:
        {
            WLMoveSurfaceRequest *req;

            req = (WLMoveSurfaceRequest*)msg->payload;

            WLSurface *surface;

            surface = WLFindSurface(req->surface);

            if (surface)
            {
                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );

                surface->x = req->x;
                surface->y = req->y;

                AddDirtyRect(
                    surface->x,
                    surface->y,
                    surface->width,
                    surface->height
                );
            }

            break;
        }  
        case WL_REDRAW:
        {
            WLCommitRequest *req;

            req = (WLCommitRequest*)msg->payload;


            WLSurface *surface;

            surface = WLFindSurface(req->surface);


            if(surface)
            {
                DrawSurface(surface);
            }

            break;
        }
        case WL_POINTER_MOTION:
        {
            WLPointerMotionEvent *event = (WLPointerMotionEvent*)msg->payload;

            break;
        }

        case WL_POINTER_BUTTON:
        {
            WLPointerButtonEvent *event = (WLPointerButtonEvent*)msg->payload;

            break;
        }
    }
}

void SaveSurfaceBackground(WLSurface *surface)
{
    DWORD *framebuffer =
        (DWORD*)GetFramebuffer();

    for(int y = 0;
        y < surface->height;
        y++)
    {
        memcpy(
            &surface->background[
                y * surface->width
            ],

            &framebuffer[
                (surface->y + y) * WSCREEN
                + surface->x
            ],

            surface->width * sizeof(DWORD)
        );
    }
}

void RestoreSurfaceBackground(WLSurface *surface)
{
    DWORD *framebuffer =
        (DWORD*)GetFramebuffer();

    for(int y = 0;
        y < surface->height;
        y++)
    {
        memcpy(
            &framebuffer[
                (surface->y + y) * WSCREEN
                + surface->x
            ],

            &surface->background[
                y * surface->width
            ],

            surface->width * sizeof(DWORD)
        );
    }
}

void ClampSurfacePosition(WLSurface* surface)
{
    // Limite esquerdo
    if(surface->x < 0)
        surface->x = 0;

    // Limite superior
    if(surface->y < 0)
        surface->y = 0;

    // Limite direito
    if(surface->x + surface->width > WSCREEN)
        surface->x = WSCREEN - surface->width;

    // Limite inferior
    if(surface->y + surface->height > HSCREEN)
        surface->y = HSCREEN - surface->height;
}

void WLPointerMotion(int x, int y)
{
        /*
        int oldX = dragSurface->x;
        int oldY = dragSurface->y;

        int newX = x - dragOffsetX;
        int newY = y - dragOffsetY;

        if (oldX == newX && oldY == newY)
            return;

        // Área antiga
        AddDirtyRect(
            oldX,
            oldY,
            dragSurface->width,
            dragSurface->height
        );

        dragSurface->x = newX;
        dragSurface->y = newY;

        // Área nova
        AddDirtyRect(
            newX,
            newY,
            dragSurface->width,
            dragSurface->height
        );

        //WLCompositorUpdate();
        return;
        */

        /*
        int oldX = dragSurface->x;
        int oldY = dragSurface->y;

        int newX = x - dragOffsetX;
        int newY = y - dragOffsetY;

        AddDirtyRect(
            oldX,
            oldY,
            dragSurface->width,
            dragSurface->height
        );

        dragSurface->x = newX;
        dragSurface->y = newY;

        AddDirtyRect(
            newX,
            newY,
            dragSurface->width,
            dragSurface->height
        );
        */

        /*
        if(dragSurface)
        {
        int oldX = dragSurface->x;
        int oldY = dragSurface->y;

        int newX =
            x - dragOffsetX;

        int newY =
            y - dragOffsetY;

        if(oldX == newX &&
           oldY == newY)
        {
            return;
        }

        AddDirtyRect(
            oldX,
            oldY,
            dragSurface->width,
            dragSurface->height
        );

        dragSurface->x = newX;
        dragSurface->y = newY;

        AddDirtyRect(
            newX,
            newY,
            dragSurface->width,
            dragSurface->height
        );*/

        if (dragSurface)
        {
            int newX = x - dragOffsetX;
            int newY = y - dragOffsetY;

            if (newX == dragSurface->x &&
                newY == dragSurface->y)
            {
                return;
            }

            /*
            * 1. Restaurar a posição antiga
            */
            RestoreSurfaceBackground(
                dragSurface
            );

            /*
            * 2. Mover a superfície
            */
            dragSurface->x = newX;
            dragSurface->y = newY;

            ClampSurfacePosition(dragSurface);

            /*
            * 3. Salvar o background
            * da nova posição
            */
            SaveSurfaceBackground(
                dragSurface
            );

            /*
            * 4. Desenhar a janela
            */
            DrawSurface(
                dragSurface
            );

            return;
        }


    for(int i = 0; i < WL_MAX_SURFACES; i++)
    {
        if(!surfaces[i].used) continue;

        if(PointInsideSurface(&surfaces[i], x, y))
        {
            WLPointerMotionEvent event;

            event.surface = surfaces[i].id;
            event.x = x - surfaces[i].x;
            event.y = y - surfaces[i].y;

            SendEvent(surfaces[i].client, WL_POINTER_MOTION, &event, sizeof(event));
        }
    }
}

void WLPointerButton(int button, int pressed, int mouseX, int mouseY)
{
    #define TITLEBAR_HEIGHT 20

    if(button == MOUSE_LEFT && pressed)
    {
        for(int i = WL_MAX_SURFACES - 1; i >= 0; i--)
        {
            if(!surfaces[i].used)
                continue;

            WLSurface* surface = &surfaces[i];

            if(mouseX >= surface->x &&
               mouseX < surface->x + surface->width &&
               mouseY >= surface->y &&
               mouseY < surface->y + TITLEBAR_HEIGHT)
            {
                dragSurface = surface;

                dragOffsetX = mouseX - surface->x;
                dragOffsetY = mouseY - surface->y;

                break;
            }
        }
    }
    else if(button == MOUSE_LEFT && !pressed)
    {
        dragSurface = NULL;
    }
}

void SendEvent(int client, DWORD type, void* data, DWORD size)
{
    WLMessage msg;

    memset(&msg, 0, sizeof(msg));

    msg.header.type = type;
    msg.header.size = size;

    memcpy(msg.payload, data, size);
    send(client, &msg, sizeof(msg));
}

void WLCompositorUpdate()
{
    //UpdateExplorer();
    RenderDirtyRects();

    /*
    for(int i = 0; i < WL_MAX_SURFACES; i++)
    {
        if(!surfaces[i].used) continue;
        
        DrawSurface(&surfaces[i]);
    }
    */
    DrawMouse(0xFFFFFFFF);
}

void WLDrawRect(DWORD* buffer, int width, int height, int x, int y, int w, int h, DWORD color)
{
    for(int py = 0; py < h; py++)
    {
        for(int px = 0; px < w; px++)
        {
            int sx = x + px;
            int sy = y + py;

            if(sx < 0 || sy < 0) continue;

            if(sx >= width || sy >= height) continue;

            buffer[sy * width + sx] = color;
        }
    }
}

void DrawSurface(WLSurface* s)
{
    if(s->buffer == NULL)
    {
        return;
    }

    if(s->buffer == NULL) return;

    for(int y = 0; y < s->height; y++)
    {
        for(int x = 0; x < s->width; x++)
        {
            DWORD color = s->buffer[y * s->width + x];

            if((color >> 24) == 0)
                continue;

            SetPixel(s->x + x, s->y + y, color);
        }
    }
}

void RoundRect(DWORD* buffer, int width, int height, int x, int y, int w, int h, int radius, DWORD color)
{
    int right = x + w;
    int bottom = y + h;

    DWORD pixelColor = color;

    for(int i = y + radius; i < bottom - radius; i++)
    {
        for(int j = x + radius; j < right - radius; j++)
        {
            BufferSetPixel(buffer, width, height, j, i, pixelColor);
        }
    }

    for(int i = x + radius; i < right - radius; i++)
    {
        for(int j = y; j < y + radius; j++)
        {
            BufferSetPixel(buffer, width, height, i, j, pixelColor);
        }

        for(int j = bottom - radius; j < bottom; j++)
        {
            BufferSetPixel(buffer, width, height, i, j, pixelColor);
        }
    }

    for(int i = y + radius; i < bottom - radius; i++)
    {
        for(int j = x; j < x + radius; j++)
        {
            BufferSetPixel(buffer, width, height, j, i, pixelColor);
        }

        for(int j = right - radius; j < right; j++)
        {
            BufferSetPixel(buffer, width, height, j, i, pixelColor);
        }
    }

    for(int dx = -radius; dx <= radius; dx++)
    {
        for(int dy = -radius; dy <= radius; dy++)
        {
            if(dx * dx + dy * dy <= radius * radius)
            {
                BufferSetPixel(buffer, width, height, x + radius + dx, y + radius + dy, pixelColor);
                BufferSetPixel(buffer, width, height, right - radius - 1 + dx, y + radius + dy, pixelColor);
                BufferSetPixel(buffer, width, height, x + radius + dx, bottom - radius - 1 + dy, pixelColor);
                BufferSetPixel(buffer, width, height, right - radius - 1 + dx, bottom - radius - 1 + dy, pixelColor);
            }
        }
    }
}

WLWindow* WLCreateWindow(int x, int y, int w, int h, char* title)
{
    WLWindow *win = AllocateMemory(sizeof(WLWindow));

    win->socket = socket(AF_UNIX, SOCK_STREAM);
    connect(win->socket, "/run/wayland-0");

    win->id = nextID++;

    win->x = x;
    win->y = y;
    win->w = w;
    win->h = h;

    strcpy(win->title, title);

    win->buffer = AllocateMemory(w * h * sizeof(DWORD));

    RoundRect(win->buffer, win->w, win->h, win->curX, win->curY, win->w, win->h, 5, 0xFF1A1A1A);
    //DrawCharBuffer(win->buffer, win->w, win->h, 0, 0, 'A', 0xFFFFFFFF);
    DrawTextBuffer(win->buffer, win->w, win->h, 10, 0, "Hello, World!", 0xFFFFFFFF);
    //PrintBuffer()
    //WLPrint(win->buffer, win->w, win->h, 0, 0, "A Quick brown fox...", 0xFFFFFFFF);
    
    SendCreateSurface(win);
    SendAttachBuffer(win);
    CommitSurface(win);

    return win;
}

void SendCreateSurface(WLWindow *win)
{
    WLMessage msg;

    WLCreateSurfaceRequest req;

    memset(&msg, 0, sizeof(msg));

    req.id = win->id;
    req.x = win->x;
    req.y = win->y + 20;
    req.w = win->w;
    req.h = win->h;

    msg.header.type = WL_CREATE_SURFACE;
    msg.header.size = sizeof(req);

    memcpy(msg.payload, &req, sizeof(req));

    send(win->socket, &msg, sizeof(msg));
}

void SendAttachBuffer(WLWindow *win)
{
    WLMessage msg;
    WLAttachBuffer req;

    memset(&msg, 0, sizeof(msg));

    req.surface = win->id;
    req.buffer  = (DWORD)win->buffer;
    req.width   = win->w;
    req.height  = win->h;

    msg.header.type = WL_ATTACH_BUFFER;
    msg.header.size = sizeof(req);

    memcpy(msg.payload, &req, sizeof(req));

    send(win->socket, &msg, sizeof(msg));
}

void CommitSurface(WLWindow *win)
{
    WLMessage msg;
    WLCommit commit;

    memset(&msg, 0, sizeof(msg));

    commit.surface = win->id;

    msg.header.type = WL_COMMIT;
    msg.header.size = sizeof(commit);

    memcpy(msg.payload, &commit, sizeof(commit));

    send(win->socket, &msg, sizeof(msg));
}