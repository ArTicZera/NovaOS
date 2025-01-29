/*
    Coded by ArTic/JhoPro

    Im not going to explain this code because it's not
    about an OS development thing.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Include/math.h"

typedef struct
{
    float x, y, z;
    float speed;
    VERTEX vector;
} STAR;

#define MAXSTARS 0xFF

STAR stars[MAXSTARS];

void GenerateStars()
{
    for (int i = 0; i < MAXSTARS; i++)
    {
        stars[i].x = 0;
        stars[i].y = 0;
        stars[i].z = rand() % 100 + 1;

        stars[i].speed = 0.1;

        stars[i].vector.x = (float)(rand() % 200 - 100) / 100.0f;
        stars[i].vector.y = (float)(rand() % 200 - 100) / 100.0f;
    }
}

void UpdateStars()
{
    for (int i = 0; i < MAXSTARS; i++)
    {
        stars[i].x += stars[i].vector.x * stars[i].speed;
        stars[i].y += stars[i].vector.y * stars[i].speed;
        stars[i].z -= stars[i].speed;

        if (stars[i].z <= 0 || stars[i].x < -WSCREEN / 2 || stars[i].x > WSCREEN / 2 ||
            stars[i].y < -HSCREEN / 2 || stars[i].y > HSCREEN / 2)
        {
            stars[i].x = 0;
            stars[i].y = 0;
            stars[i].z = 100;

            stars[i].vector.x = (float)(rand() % 200 - 100) / 100.0f;
            stars[i].vector.y = (float)(rand() % 200 - 100) / 100.0f;
        }
    }
}

void _start()
{
    GenerateStars();

    int frame = 0x00;

    while (frame < (0xFF * 2))
    {
        ClearScreen();

        for (int i = 0; i < MAXSTARS; i++)
        {
            int x = (int)(stars[i].x / stars[i].z * (WSCREEN / 2)) + (WSCREEN / 2);
            int y = (int)(stars[i].y / stars[i].z * (HSCREEN / 2)) + (HSCREEN / 2);

            if (x >= 0 && x < WSCREEN && y >= 0 && y < HSCREEN)
            {
                SetPixel(x, y, 0x0F);

                SetPixel(x + 1, y, 0x0F);
                SetPixel(x, y + 1, 0x0F);
                SetPixel(x + 1, y + 1, 0x0F);

                SetPixel(x + 2, y, 0x0F);
                SetPixel(x, y + 2, 0x0F);
                SetPixel(x + 1, y + 2, 0x0F);
                SetPixel(x + 2, y + 1, 0x0F);
                SetPixel(x + 2, y + 2, 0x0F);
            }
        }

        UpdateStars();
    
        frame++;
    }
}
