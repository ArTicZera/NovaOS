#include "qjs_runtime.h"

static JSValue JS_WA_DrawRect
(
    JSContext *ctx,
    JSValueConst this_value,
    int argc,
    JSValueConst *argv
)
{
    (void)this_value;
    
    if (argc < 5)
    {
        return JS_EXCEPTION;
    }

    int32_t x, y, w, h, color;

    JS_ToInt32(ctx, &x, argv[0]);
    JS_ToInt32(ctx, &y, argv[1]);
    JS_ToInt32(ctx, &w, argv[2]);
    JS_ToInt32(ctx, &h, argv[3]);

    JS_ToInt32(ctx, &color, argv[4]);

    wa_context_t *WebArchy = (wa_context_t*)JS_GetContextOpaque(ctx);

    if (WebArchy && WebArchy->hal.draw_rect)
    {
        WebArchy->hal.draw_rect(x, y, w, h, color);
    }

    return JS_UNDEFINED;
}

VOID WA_Eval_JS(wa_context_t *ctx, const CHAR* script)
{
    size_t len = 0;
    while (script[len]) len++;

    JSValue val =
    JS_Eval
    (
        ctx->js_ctx, script,
        len, "<inline>", JS_EVAL_TYPE_GLOBAL
    );

    if (JS_IsException(val))
    {
        JSValue exception_val = JS_GetException(ctx->js_ctx);
        JS_FreeValue(ctx->js_ctx, exception_val);
    }

    JS_FreeValue(ctx->js_ctx, val);
}