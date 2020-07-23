#include <paddle.h>
#include <Arduboy2.h>

/**
 * Begin because thats what arduino wants :)
 */
void Paddle::begin()
{
    reset();
}

/**
 * getters
 */
int16_t Paddle::getX()
{
    return x;
}
int16_t Paddle::getY()
{
    return y;
}
uint8_t Paddle::getWidth()
{
    return w;
}
uint8_t Paddle::getHeight()
{
    return h;
}

/**
 * Reset paddle few pixels above bottom center
 */
void Paddle::reset()
{
    // WIDTH of the screen divided by 2 minus half of the paddle width
    x = WIDTH / 2 - w / 2;
    // HEIGHT of the screen plus the height of the paddle
    y = HEIGHT - h - 0;
}

/**
 * Handle button inputs
 */
void Paddle::handleButtons(uint8_t buttons)
{
    if (buttons & LEFT_BUTTON)
    {
        // if we are not fully left yet
        if (x > 0)
            // move left
            --x;
    }
    else if (buttons & RIGHT_BUTTON)
    {
        // if we are not fully right (SCREEN WIDTH - paddle width)
        if (x < WIDTH - w)
            // move right
            ++x;
    }
}

/**
 * Draw the paddle
 */
void Paddle::draw(Arduboy2 &arduboy)
{
    // draw a filled rectangle in x,y position width and height
    arduboy.fillRect(x, y, w, h, WHITE);
}