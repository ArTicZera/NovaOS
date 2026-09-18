#include "render.h"

static VOID Render_node
(
    wa_context_t *ctx,
    wa_dom_node_t *node,
    DWORD *offset_y
)
{
    if (!node) return;

    if (node->type == NODE_ELEMENT)
    {
        // Your function to Draw Rectangle

        *offset_y += 10;
    }

    else if (node->type == NODE_TEXT)
    {
        // Your function to Print String

        *offset_y += 20;
    }

    if (node->first_child)
    {
        Render_node(ctx, node->first_child, offset_y);
    }

    if (node->next_sibling)
    {
        Render_node(ctx, node->next_sibling, offset_y);
    }
}