/**
 * Simple pong example game for arduboy
 */
#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

/**
 * Variables used in this program
 */
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
 * Starting a new game
 */
void newGame()
{
  // reset both scores
  p1score = 0;
  p2score = 0;
  // reset ball
  resetBall();
}

/**
 * The first thing that happens when arduboy is started
 * All the initialization etc happens here
 */
void setup()
{
  // initialize arduboy
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(60);
  arduboy.clear();

  // start a new game
  newGame();
}

/**
 * Draw a playfield and scores
 */
void drawPlayField()
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

  // draw ball
  arduboy.fillCircle(ballx, bally, ballr);

  // draw both paddles
  arduboy.fillRect(p1x, p1y, p1width, p1height);
  arduboy.fillRect(p2x, p2y, p2width, p2height);
}

/**
 * wait for either player to start the game
 */
void titleScreen()
{
  if (
      arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(A_BUTTON) ||
      arduboy.justPressed(B_BUTTON))
  {
    gamestate = GAME_PLAY;
  }
}

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

  // if ball is over the left or right side increase points and resetball
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
 * main game logic
 */
void gameTick()
{

  handleBall();
  handlePaddles();

  // check if either side won
  if (p1score >= maxpoints || p2score >= maxpoints)
  {
    gamestate = GAME_WON;
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
  delay(500);

  if (
      arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) ||
      arduboy.pressed(B_BUTTON))
  {
    // start a new game
    newGame();
    // right away
    gamestate = GAME_PLAY;
  }
}

/**
 * This will run again and again and again
 */
void loop()
{
  if (!arduboy.nextFrame())
    return;
  arduboy.clear();
  // this is required for justPressed
  arduboy.pollButtons();

  drawPlayField();

  switch (gamestate)
  {
  // title screen
  case GAME_TITLE:
    titleScreen();
    break;
  // when game is running always run these
  case GAME_PLAY:
    gameTick();
    break;
  // game ends
  case GAME_WON:
    winScreen();
    break;
  }

  arduboy.display();
}