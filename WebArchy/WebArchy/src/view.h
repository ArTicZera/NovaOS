#include "defs.h"
#include "browser.h"

#define MAX_HISTORY 16
#define MAX_SOURCE_SIZE 65536

typedef int
(*wa_fetch_resource_fn)
(const char *uri, char *out_buffer, size_t max_len);

typedef enum
{
    NAV_STATE_IDLE,
    NAV_STATE_LOADING,
    NAV_STATE_COMPLETE,
    NAV_STATE_FAILED
} wa_nav_state_t;

typedef struct
{
    wa_context_t *ctx;
    
    char history[MAX_HISTORY][128];
    int history_index;
    int history_count;

    char *current_source_buffer;
    wa_nav_state_t state;

    wa_fetch_resource_fn fetch_cb;
} wa_webview_t;

wa_webview_t* bw_view_create
(
    wa_hall_t hal,
    uint32_t w,
    uint32_t h,
    wa_fetch_resource_fn fetch_cb
);

void wa_view_navigate(wa_webview_t *view, const char *uri);
void wa_view_reload(wa_webview_t *view);
void wa_view_go_back(wa_webview_t *view);
void wa_view_go_forward(wa_webview_t *view);
void wa_view_stop(wa_webview_t *view);
void wa_view_destroy(wa_webview_t *view);