#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

// gamestate
const int GAME_TITLE = 0;
const int GAME_PLAY = 1;
const int GAME_WON = 2;
uint8_t gamestate = GAME_TITLE;

// ball initial position center of the screen
int16_t ballx = WIDTH / 2;
int16_t bally = HEIGHT / 2;
// ball velocity in x and y direction
int8_t vx = 1;
int8_t vy = 1;
// ball radius
const int8_t ballr = 2;

// player1 paddle
int16_t p1width = 3;
int16_t p1height = 10;
int16_t p1y = HEIGHT / 2 - p1height / 2;
const int16_t p1x = 0;

// player2 paddle
int16_t p2width = 3;
int16_t p2height = 10;
int16_t p2y = HEIGHT / 2 - p2height / 2;
const int16_t p2x = WIDTH - p2width;

// score
uint16_t p1score = 0;
uint16_t p2score = 0;
// maxpoints
const uint16_t maxpoints = 5;

void setup()
{
  // start using arduboy library
  arduboy.begin();
  // initialize random number generator
  arduboy.initRandomSeed();
  // set framerate to 60 frames per second
  arduboy.setFrameRate(60);
  // clear the screen just in case
  arduboy.clear();
}

/**
 * Reset ball position to the center of the screen
 * and randomize direction the ball will start to
 * go
 */
void resetBall()
{
  // move the ball to the center of the screen
  ballx = WIDTH / 2;
  bally = HEIGHT / 2;

  // randomize the ball starting direction
  if (random(0, 2))
  {
    vx = -vx;
  }
  if (random(0, 2))
  {
    vy = -vy;
  }
}

/**
 * Draw everything we have on the screen
 */
void draw()
{
  // spacing for the line
  // half will be white half black
  uint16_t spacing = 6;
  for (int i = 0; i < HEIGHT; i += spacing)
  {
    arduboy.drawLine(
        WIDTH / 2, i,              // start coordinate
        WIDTH / 2, i + spacing / 2 // end coordinate
    );
  }

  // print both scores
  // textsize 2 is 12x16 px
  arduboy.setTextSize(2);

  // player1 score on the left side
  arduboy.setCursor(20, 5);
  arduboy.print(p1score);

  // player2 score on the right side
  arduboy.setCursor(WIDTH - 20 - 12, 5);
  arduboy.print(p2score);

  // draw the ball
  arduboy.fillCircle(ballx, bally, ballr);

  // draw both paddles
  arduboy.fillRect(p1x, p1y, p1width, p1height);
  arduboy.fillRect(p2x, p2y, p2width, p2height);
}

/**
 * Check if keys are pressed and move the player paddles
 */
void handlePaddles()
{
  // player1 paddle
  if (arduboy.pressed(UP_BUTTON))
  {
    if (p1y > 0)
    {
      p1y--;
    }
  }
  if (arduboy.pressed(DOWN_BUTTON))
  {
    if (p1y + p1height < HEIGHT)
    {
      p1y++;
    }
  }
  // player 2 paddle
  if (arduboy.pressed(B_BUTTON))
  {
    if (p2y > 0)
    {
      p2y--;
    }
  }
  // player 2 paddle
  if (arduboy.pressed(A_BUTTON))
  {
    if (p2y + p2height < HEIGHT)
    {
      p2y++;
    }
  }
}

/**
 * Ball movement, bounce from top and bottom and from paddles
 * Check if ball is over a side and increase points if it is
 */
void handleBall()
{
  // move the ball
  ballx += vx;
  bally += vy;

  // keep the ball inside our playfield
  if (
      bally - ballr <= 0         // bounce from the top
      || bally + ballr >= HEIGHT // and from the bottom
  )
  {
    // reverse up down
    vy = -vy;
  }

  // check if we hit either paddle
  // player1 paddle
  if (ballx - ballr == p1x + p1width    // ball is at the paddle surface
      && bally + ballr > p1y            // and between the paddle top position
      && bally - ballr < p1y + p1height // and between the paddle bottom position
  )
  {
    sound.tone(NOTE_A4, 50);
    vx = -vx;
  }
  // and player2 paddle
  if (ballx + ballr == p2x              // ball is at the paddle surface
      && bally + ballr > p2y            // and between the paddle position
      && bally - ballr < p2y + p2height // and between the paddle position
  )
  {
    sound.tone(NOTE_A4, 50);
    vx = -vx;
  }

  // if ball is over the left or right side resetball
  // and increase points
  // over right side
  if (ballx + ballr < 0)
  {
    p2score++;
    resetBall();
  }
  if (ballx - ballr > WIDTH)
  {
    p1score++;
    resetBall();
  }
}

/**
 * wait for either player to start the game
 */
void titleScreen()
{
  arduboy.pollButtons();
  if (
      arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(A_BUTTON) ||
      arduboy.justPressed(B_BUTTON))
  {
    resetBall();
    gamestate = GAME_PLAY;
  }
}

/**
 * Show which player won and
 * get ready for next round
 */
void winScreen()
{
  // textsize 2 is 12x16 px
  arduboy.setTextSize(2);

  // print who won about center of the screen
  arduboy.setCursor(34, 23);
  if (p1score > p2score)
  {
    arduboy.print("P1");
  }
  else
  {
    arduboy.print("P2");
  }
  arduboy.print(" won");

  // display right away
  arduboy.display();
  // and sleep for a moment so that there is time to react and not start a new game immediately
  delay(500);

  arduboy.pollButtons();
  if (
      arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) ||
      arduboy.pressed(B_BUTTON))
  {
    // start a new game
    resetBall();
    p1score = 0;
    p2score = 0;
    gamestate = GAME_PLAY;
  }
}

void loop()
{
  // wait untill next frame
  if (!arduboy.nextFrame())
    return;
  // clear the screen
  arduboy.clear();

  // everything we need to do goes here
  // always draw the playfield ball and paddles
  draw();

  // dependig on the state we will do different things
  switch (gamestate)
  {
  // title screen, waits for input and moves to GAME_PLAY
  case GAME_TITLE:
    titleScreen();
    break;
  // game is playing untill either side wins, moves to GAME_WON
  case GAME_PLAY:
    handlePaddles();
    handleBall();
    if (p1score >= maxpoints || p2score >= maxpoints)
    {
      // go to win state
      gamestate = GAME_WON;
    }
    break;
  // game won by a player
  case GAME_WON:
    winScreen();
    break;
  }

  // flip the buffer to show what we draw
  arduboy.display();
}