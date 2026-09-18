#include <WebArchy.h>

VOID DrawRect(INT x, INT y, INT w, INT h, DWORD color);
VOID DrawString(CONST CHAR* str, DWORD color);

LPVOID WB_Alloc(size_t bytes)
{
    return malloc(bytes);
}

VOID WB_Free(LPVOID ptr)
{
    free(ptr);
}

int kernel_fetch_resource(const char *uri, char *out_buffer, size_t max_len) {
    printf("\n[NET STACK] Requisitando recurso: %s\n", uri);

    const char *mock_html_page1 = 
        "<html><body>"
        "<h1>Página Inicial - BareWeb</h1>"
        "<script>"
        "  drawRect(20, 100, 300, 50, 0xFF00FF00);" // Desenha caixa verde via JS
        "</script>"
        "</body></html>";

    const char *mock_html_page2 = 
        "<html><body>"
        "<h1>Segunda Pagina (Navegacao OK)</h1>"
        "<script>"
        "  drawRect(20, 200, 150, 80, 0xFFFF0000);" // Desenha caixa vermelha via JS
        "</script>"
        "</body></html>";

    const char *source_to_use = mock_html_page1;

    if (strcmp(uri, "http://bareweb.os/page2") == 0) {
        source_to_use = mock_html_page2;
    }

    size_t len = strlen(source_to_use);
    if (len >= max_len) len = max_len - 1;

    strncpy(out_buffer, source_to_use, len);
    out_buffer[len] = '\0';

    return (int)len;
}

// --- 3. LOOP DE EVENTOS E TESTE DO NAVEGADOR ---

void print_menu(void) {
    printf("\n==================================================\n");
    printf("         BAREWEB OS BROWSER TEST RUNNER           \n");
    printf("==================================================\n");
    printf("Comandos do Teclado Simulado:\n");
    printf(" [1] Navegar para 'http://bareweb.os/home'\n");
    printf(" [2] Navegar para 'http://bareweb.os/page2'\n");
    printf(" [r] Recarregar pagina (Reload - F5)\n");
    printf(" [b] Voltar no historico (Go Back)\n");
    printf(" [f] Avancar no historico (Go Forward)\n");
    printf(" [e] Executar script JS dinamico na pagina\n");
    printf(" [q] Sair do sistema\n");
    printf("--------------------------------------------------\n");
    printf("Opcao > ");
}

int main(void) {
    // Configura os ponteiros de abstração de hardware
    bw_hal_t hal = {
        .draw_rect = kernel_gfx_draw_rect,
        .draw_text = kernel_gfx_draw_text,
        .alloc = kernel_alloc,
        .free = kernel_free
    };

    // Cria a instância do navegador (WebView2 estilo bare-metal)
    bw_webview_t *browser = wa_view_create(hal, 1024, 768, kernel_fetch_resource);
    if (!browser) {
        printf("[ERRO] Falha ao inicializar a biblioteca BareWeb!\n");
        return 1;
    }

    // Carrega a página inicial padrão
    wa_view_navigate(browser, "http://bareweb.os/home");

    char input[32];
    int running = 1;

    while (running) {
        print_menu();
        if (!fgets(input, sizeof(input), stdin)) break;

        char cmd = input[0];
        switch (cmd) {
            case '1':
                wa_view_navigate(browser, "https://customer.lingscars.com/"); // test site (lingscars)
                break;
            case '2':
                wa_view_navigate(browser, "https://www.google.com/"); // other test site (google)
                break;
            case 'r':
            case 'R':
                printf("\n[EVENTO] F5 pressionado. Atualizando pagina...\n");
                wa_view_reload(browser);
                break;
            case 'b':
            case 'B':
                printf("\n[EVENTO] Botao Voltar pressionado...\n");
                wa_view_go_back(browser);
                break;
            case 'f':
            case 'F':
                printf("\n[EVENTO] Botao Avancar pressionado...\n");
                wa_view_go_forward(browser);
                break;
            case 'e':
            case 'E':
                printf("\n[EVENTO] Executando JS dinamicamente no contexto atual...\n");
                WA_Eval_JS(browser->ctx, "drawRect(500, 500, 100, 100, 0xFF0000FF);");
                break;
            case 'q':
            case 'Q':
                running = 0;
                break;
            default:
                printf("\nComando desconhecido.\n");
                break;
        }
    }

    // Libera os recursos alocados pela engine
    wa_view_destroy(browser);
    printf("\n[WebArchy] Instancia destruida com sucesso. Saindo...\n");

    return 0;
}
