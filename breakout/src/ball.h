#pragma once

#include <Arduboy2.h>
// we need to know about the paddle and bricks so that we know
// how to handle hitting them
#include <paddle.h>

/**
 * Ball
 */
class Ball
{
public:
    Ball(uint8_t radius) {
        r = radius;
    }
    void begin();
    void reset();
    void handleButtons(uint8_t buttons);
    bool update();
    void checkHit(Paddle &paddle);
    bool checkHit(Arduboy2 &arduboy, Rect r);
    void draw(Arduboy2 &arduboy);

private:
    void boundToViewPort();
    // circle xy are from the circle center
    int16_t x;
    int16_t y;
    uint8_t r;
    // speed of the ball
    int16_t dx;
    int16_t dy;
};