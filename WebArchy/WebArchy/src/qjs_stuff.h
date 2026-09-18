#include "qjs_include.h"
#include "view.h"

static JSValue js_location_reload
(
    JSContext *ctx, JSValueConst this_val,
    int argc, JSValueConst *argv
);

void register_browser_control_api(wa_webview_t *view);