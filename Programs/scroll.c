#include "../Include/stdint.h"
#include "../Include/math.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"

#define WAVE_AMPLITUDE 10 // Amplitude da onda
#define WAVE_FREQUENCY 30 // Frequência da onda
#define SCROLL_SPEED 1     // Velocidade de rolagem

void _start()
{
    const char* message = "Thanks for trying NovaOS!!!";
    int messageLength = 27;
    int initialX = WSCREEN;

    while (initialX > -(27 * 8 * 3))
    {
        ClearScreen();

        for (int i = 0; i < messageLength; i++)
        {
            int charX = initialX + (i * 8);
            int yOffset = (int)(WAVE_AMPLITUDE * sin((charX + WSCREEN) / WAVE_FREQUENCY));

            SetCursorX(charX);
            SetCursorY((HSCREEN / 2) + yOffset);

            PrintOut(message[i], 0x20 + i);
        }

        initialX -= SCROLL_SPEED;

        for (volatile int i = 0; i < 100000; i++);
    }
}
