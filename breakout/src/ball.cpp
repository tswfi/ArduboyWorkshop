#include <ball.h>
#include <Arduboy2.h>

/**
 * Begin because thats what arduino wants :)
 */
void Ball::begin()
{
    reset();
}

/**
 * Reset Ball few pixels above bottom center
 */
void Ball::reset()
{
    // speed to 0
    dx = 0;
    dy = 0;
    // position lower center
    x = WIDTH / 2;
    y = HEIGHT / 3 * 2;
}

/**
 * Handle button inputs
 */
void Ball::handleButtons(uint8_t buttons)
{
    if (buttons & A_BUTTON)
    {
        if (dx == 0 && dy == 0)
        {
            // launch the ball
            dx = -1;
            dy = -1;
        }
    }
}

bool Ball::update()
{
    // move according to our speed
    x = x + dx;
    y = y + dy;

    // and then check if we hit the walls
    Ball::boundToViewPort();

    // and check if we fell of the world
    if (y + r > HEIGHT)
    {
        return false;
    }
    return true;
}

void Ball::boundToViewPort()
{
    // hitting the right wall
    if (x - r <= 0)
    {
        dx = -dx; // change direction
        x = r;    // and make sure we stay inside the frame
    }
    // hitting the left wall
    if (x + r >= WIDTH)
    {
        dx = -dx;
        x = WIDTH - r;
    }
    // hitting top wall
    if (y - r <= 0)
    {
        dy = -dy;
        y = r;
    }

    // hitting bottom wall, good for testing :)
    /*
    if (y + r >= HEIGHT)
    {
        dy = -dy;
        y = HEIGHT - r;
    }
    */
}

/**
 * check if we hit the paddle
 */
void Ball::checkHit(Paddle &paddle)
{
    // we only need to check the paddle top surface
    // so if ball bottom is at the same level as paddle top level
    // and balls right side is paddles left side AND balls left side is
    // inside paddles right side
    if (
        y + r == paddle.getY()                        // Y position, ball bottom is at the same level as paddles top
        && x + r > paddle.getX()                      // ball right side is larger than paddle X position
        && x - r < paddle.getX() + paddle.getWidth()) // ball left side is smaller than paddle X + width
    {
        // HIT, change direction
        dy = -dy;
        // and make sure we are at the right height
        y = paddle.getY() - r;
    }
}

/**
 * check hits with brick rects
 */
bool Ball::checkHit(Arduboy2 &arduboy, Rect c)
{
    Rect ballRect = Rect(x - r, y - r, r * 2, r * 2);

    if (arduboy.collide(ballRect, c))
    {
        // we hit the brick, now check where the hit came from

        // if the ball is between the x coordinates the hit came from
        // either top or bottom
        if (x > c.x && x < c.x + c.width)
        {
            // back up or down
            // the case is the same for top and bottom of the brick
            dy = -dy;
            y += dy;
        }
        // if the ball is between y coordinates the hit came from the side
        else if (y > c.y && y < c.y + c.height)
        {
            // change direction to left or right,
            // same case for both sides
            dx = -dx;
            x += dx;
        }
        // and if it was neither we know it was the corner
        else
        {
            // in the corner we can get fancy
            // if we were coming in from the right and hit the bottom right
            // corner we want to send the ball back where it came from
            // if it was still coming from the right but hit the top right
            // corner we want to just flip the y

            // calculate difference for the corners
            int16_t cx = abs(x - c.x) - c.width / 2;
            int16_t cy = abs(y - c.y) - c.height / 2;

            // the different corners will have different cx cy values
            // bottom right both are positive
            // bottom left cx is negative and cy is positive
            // top right cx is positive and cy is negative
            // top left both are negative

            // all the special cases
            if (
                dx > 0 && dy > 0 && cx < 0 && cy < 0    // down and right and the hit came from top left corner
                || dx > 0 && dy < 0 && cx > 0 && cy < 0 // down and left and hit came from top right corner
                || dx < 0 && dy < 0 && cx > 0 && cy > 0 // up and left and hit came from bottom right corner
                || dx < 0 && dy > 0 && cx < 0 && cy > 0 // up and right and hit came from bottom left corner
            )
            {
                dx = -dx;
                dy = -dy;
                x += dx;
                y += dy;
            }
            // default to up down deflection
            else
            {
                dy = -dy;
                y += dy;
            }
        }

        // let main handle the hit changes to other things
        return true;
    }

    return false;
}

/**
 * Draw the Ball
 */
void Ball::draw(Arduboy2 &arduboy)
{
    // draw a filled rectangle in x,y position width and height
    arduboy.fillCircle(x, y, r, WHITE);
}