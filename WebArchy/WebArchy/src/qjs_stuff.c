#include "qjs_stuff.h"
#include "defs.h"

static JSValue js_location_reload
(
    JSContext *ctx, JSValueConst this_val,
    int argc, JSValueConst *argv
)
{
    (void)this_val; (void)argc; (void)argv;
    
    wa_context_t *wa_ctx = (wa_context_t*)JS_GetContextOpaque(ctx);
    wa_webview_t *view = (wa_webview_t*)wa_ctx->user_data; 

    if (view)
    {
        wa_view_reload(view);
    }
    
    return JS_UNDEFINED;
}

void register_browser_control_api(wa_webview_t *view)
{
    JSContext *ctx = view->ctx->js_ctx;
    JSValue global_obj = JS_GetGlobalObject(ctx);
    
    JSValue location_obj = JS_NewObject(ctx);

    JS_SetPropertyStr
    (
        ctx, location_obj,
        "reload",
        JS_NewCFunction
        (
            ctx, js_location_reload,
            "reload", 0
        )
    );
    
    JS_SetPropertyStr
    (
        ctx, global_obj,
        "location",
        location_obj
    );

    JS_FreeValue(ctx, global_obj);
}