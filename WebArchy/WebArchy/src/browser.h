#include "../include/WebArchy.h"
#include "defs.h"

wa_context_t* WA_Init
(
    wa_hall_t hal,
    DWORD w, DWORD h
);

VOID WA_Load_HTML(wa_context_t *ctx, const CHAR *html_source);
VOID WA_Render(wa_context_t *ctx);
VOID WA_Destroy(wa_context_t *ctx);