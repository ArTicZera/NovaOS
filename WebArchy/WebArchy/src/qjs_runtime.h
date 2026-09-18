#include "defs.h"

static JSValue JS_WA_DrawRect
(
    JSContext *ctx,
    JSValueConst this_value,
    int argc,
    JSValueConst *argv
);

VOID WA_Eval_JS(wa_context_t *ctx, const CHAR* script);