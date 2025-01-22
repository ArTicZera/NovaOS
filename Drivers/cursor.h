/*
    Coded by ArTic/JhoPro

    Each bit (0/1) represents a pixel.
    - 0 : Skip drawing in the pixel.
    - 1 : Draws a white pixel

    In the end it will result in a cursor.
*/

#define WCURSOR 12
#define HCURSOR 19

//BIT 1 = White
//BIT 0 = Black
BYTE cursor[] = 
{
    0b10000000, 0b00000000,
    0b11000000, 0b00000000,
    0b11100000, 0b00000000,
    0b11110000, 0b00000000,
    0b11111000, 0b00000000,
    0b11111100, 0b00000000,
    0b11111110, 0b00000000,
    0b11111111, 0b00000000,
    0b11111111, 0b10000000,
    0b11111111, 0b11000000,
    0b11111111, 0b11100000,
    0b11111111, 0b11110000,
    0b11111111, 0b11110000,
    0b11111111, 0b00000000,
    0b11110111, 0b10000000,
    0b11100111, 0b10000000,
    0b11000011, 0b11000000,
    0b00000011, 0b11000000,
    0b00000001, 0b10000000
};
