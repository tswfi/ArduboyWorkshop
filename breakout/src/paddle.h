#pragma once

#include <Arduboy2.h>

/**
 * Player paddle
 */
class Paddle
{
public:
    Paddle(uint8_t width, uint8_t height) {
        w = width;
        h = height;
    }
    void begin();
    void reset();
    void handleButtons(uint8_t buttons);
    void draw(Arduboy2 &arduboy);
    int16_t getX();
    int16_t getY();
    uint8_t getWidth();
    uint8_t getHeight();

private:
    int16_t x;
    int16_t y;
    uint8_t w;
    uint8_t h;
};