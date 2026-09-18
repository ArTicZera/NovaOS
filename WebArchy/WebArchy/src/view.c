#include "view.h"
#include "others.h"

wa_webview_t* wa_view_create
(
    wa_hall_t hal,
    uint32_t width,
    uint32_t height,
    wa_fetch_resource_fn fetch_cb
)
{
    if (!hal.alloc) return NULL;

    wa_webview_t *view =
    (wa_webview_t*)hal.alloc(sizeof(wa_webview_t));
    if (!view) return NULL;

    view->ctx = WA_Init(hal, width, height);
    view->fetch_cb = fetch_cb;
    view->history_index = -1;
    view->history_count = 0;
    view->state = NAV_STATE_IDLE;
    view->current_source_buffer = (char*)hal.alloc(MAX_SOURCE_SIZE);

    return view;
}

static void wa_internal_load_current(wa_webview_t *view)
{
    if (!view || view->history_index < 0 || !view->fetch_cb) return;

    view->state = NAV_STATE_LOADING;
    const char *current_uri = view->history[view->history_index];

    int bytes_read = view->fetch_cb
    (
        current_uri,
        view->current_source_buffer,
        MAX_SOURCE_SIZE - 1
    );

    if (bytes_read <= 0)
    {
        view->state = NAV_STATE_FAILED;
        return;
    }

    view->current_source_buffer[bytes_read] = '\0';
    WA_Load_HTML(view->ctx, view->current_source_buffer);
    WA_Render(view->ctx);
    view->state = NAV_STATE_COMPLETE;
}

void wa_view_navigate(wa_webview_t *view, const char *uri)
{
    if (!view || !uri) return;

    if (view->history_index + 1 < MAX_HISTORY)
    {
        view->history_index++;
        WA_Strcmp(view->history[view->history_index], uri);
        view->history_count = view->history_index + 1;
    }

    wa_internal_load_current(view);
}

void wa_view_reload(wa_webview_t *view)
{
    wa_internal_load_current(view);
}

void wa_view_go_back(wa_webview_t *view)
{
    if (!view) return;

    if (view->history_index > 0)
    {
        view->history_index--;
        wa_internal_load_current(view);
    }
}

void wa_view_go_forward(wa_webview_t *view)
{
    if (!view) return;

    if (view->history_index + 1 < view->history_count)
    {
        view->history_index++;
        wa_internal_load_current(view);
    }
}

void wa_view_stop(wa_webview_t *view)
{
    if (!view) return;

    if (view->state == NAV_STATE_LOADING)
    {
        view->state = NAV_STATE_IDLE;
    }
}

void wa_view_destroy(wa_webview_t *view)
{
    if (!view) return;
    wa_hall_t hal = view->ctx->hal;
    WA_Destroy(view->ctx);
    hal.free(view->current_source_buffer);
    hal.free(view);
}