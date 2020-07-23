#include <Arduino.h>
#include <Arduboy2.h>

// our arduboy library
Arduboy2 arduboy;

// gamestate
const int GAME_TITLE = 0;
const int GAME_PLAY = 1;
const int GAME_OVER = 2;
const int GAME_WON = 3;
uint8_t gamestate = GAME_TITLE;

// paddle logic in different file
#include <paddle.h>
const int paddle_width = 15;
const int paddle_height = 3;
Paddle paddle(paddle_width, paddle_height);

// ball logic in different file
#include <ball.h>
const int ball_radius = 2;
Ball ball(ball_radius);

// bricks
const int BRICK_COLUMNS = 6;
const int BRICK_ROWS = 3;
const int BRICK_WIDTH = ((WIDTH - BRICK_COLUMNS) / BRICK_COLUMNS);
const int BRICK_XSPACING = 1;
const int BRICK_YSPACING = 1;
const int BRICK_HEIGHT = ((HEIGHT / 2 / BRICK_ROWS) - BRICK_ROWS);
uint8_t bricks[BRICK_COLUMNS][BRICK_ROWS];

/**
 * Randomize a new level on every startup
 */
void randomizeLevel()
{
  for (uint8_t x = 0; x < BRICK_COLUMNS; x++)
  {
    for (uint8_t y = 0; y < BRICK_ROWS; y++)
    {
      bricks[x][y] = random(0, 3); // strength of the block: either 0, 1 or 2
    }
  }
}

/**
 * Reset game to a new game
 */
void newGame()
{
  // reset paddle to default position
  paddle.reset();
  // reset ball
  ball.reset();

  // reset bricks by creating a new level
  randomizeLevel();
}

/**
 * Arduino always runs this for us first
 */
void setup()
{
  // reset arduboy
  arduboy.begin();
  // make sure we have a "random" number if we want one
  arduboy.initRandomSeed();
  // framerate for the game, because 60fps is good :)
  arduboy.setFrameRate(60);
  // clear the screen
  arduboy.clear();

  // make sure paddle has been "started"
  paddle.begin();

  // initialize bricks for this level
  randomizeLevel();

  // run "newGame" just like when game is lost and we
  // start a new game
  newGame();
}

/**
 * All input handled here
 */
void handleInput(uint8_t buttons)
{
  // let paddle do what it wants with the buttons
  paddle.handleButtons(buttons);
  ball.handleButtons(buttons);
}

int16_t getBrickX(uint8_t x)
{
  return (x * BRICK_WIDTH) + (x * BRICK_XSPACING);
}
int16_t getBrickY(uint8_t y)
{
  return (y * BRICK_HEIGHT) + (y * BRICK_YSPACING);
}

/**
 * Call draw on all of our objects
 */
void handleDraw(Arduboy2 &arduboy)
{

  // call draw on our paddle
  paddle.draw(arduboy);

  // call draw on our ball
  ball.draw(arduboy);

  // and draw all bricks
  for (uint8_t x = 0; x < BRICK_COLUMNS; x++)
  {
    for (uint8_t y = 0; y < BRICK_ROWS; y++)
    {
      switch (bricks[x][y])
      {
      case 0:
        // no brick here, do nothing
        break;

      case 1:
        // brick about to brake, draw a rectangle
        arduboy.drawRect(
            getBrickX(x),
            getBrickY(y),
            BRICK_WIDTH,
            BRICK_HEIGHT,
            WHITE);
        break;

      default:
        arduboy.fillRect(
            getBrickX(x),
            getBrickY(y),
            BRICK_WIDTH,
            BRICK_HEIGHT,
            WHITE);
        break;
      }
    }
  }
}

/**
 * Check if the ball hit any of the bricks
 */
void checkBrickHits()
{
  for (uint8_t x = 0; x < BRICK_COLUMNS; x++)
  {
    for (uint8_t y = 0; y < BRICK_ROWS; y++)
    {
      if (bricks[x][y] == 0)
      {
        // no brick here
        continue;
      }
      Rect brickRect = Rect(
          getBrickX(x),
          getBrickY(y),
          BRICK_WIDTH,
          BRICK_HEIGHT);
      if (ball.checkHit(arduboy, brickRect))
      {
        // ball hit the brick, decrease its points
        bricks[x][y]--;
      }
    }
  }
}

bool checkWin()
{
  for (uint8_t x = 0; x < BRICK_COLUMNS; x++)
  {
    for (uint8_t y = 0; y < BRICK_ROWS; y++)
    {
      if (bricks[x][y] > 0)
        return false;
    }
  }
  return true;
}

/**
 * when game is running update our ball
 * check if the ball is still in the game
 * and if it collides with anything
 */
void gameTick()
{
  // ball update returns false when the ball falls of the world
  if (!ball.update())
  {
    gamestate = GAME_OVER;
  }
  // check if we hit the paddle
  ball.checkHit(paddle);

  // check hits with bricks
  checkBrickHits();

  // check if all the blocks have been destroyed
  if (checkWin())
  {
    gamestate = GAME_WON;
  }
}

void printCenter(Arduboy2 &arduboy, const char *txt)
{
  uint8_t txtwidth = strlen(txt) * arduboy.getTextSize() * 6; // size 1 is 6x8 pixels, size 2 is 12x16
  uint8_t txtheight = arduboy.getTextSize() * 8;
  arduboy.setCursor(WIDTH / 2 - txtwidth / 2, HEIGHT / 2 - txtheight / 2);
  arduboy.print(txt);
}

/**
 * Welcome screen to show before the game starts
 */
void titlescreen()
{
  printCenter(arduboy, "BREAKOUT");
  if (arduboy.justPressed(A_BUTTON))
  {
    gamestate = GAME_PLAY;
  }
}

/**
 * Oh no, ball fell of the world
 */
void gameoverscreen()
{
  printCenter(arduboy, "Game Over!");
  if (arduboy.justPressed(A_BUTTON))
  {
    newGame();
    gamestate = GAME_PLAY;
  }
}

void gamewonscreen()
{
  printCenter(arduboy, "YOU WIN!");
  if (arduboy.justPressed(A_BUTTON))
  {
    newGame();
    gamestate = GAME_PLAY;
  }
}

void loop()
{
  // not running too fast
  // arduboy library will check if enough time has passed and the next frame
  // should be done
  if (!arduboy.nextFrame())
    return;

  // start by clearing the screen completely
  arduboy.clear();

  // this is required for justPressed
  arduboy.pollButtons();

  // do different things according to our gamestate
  switch (gamestate)
  {
  case GAME_TITLE:
    titlescreen();
    break;

  case GAME_PLAY:
    // when game is running always run these
    handleInput(arduboy.buttonsState());
    gameTick();
    break;

  case GAME_OVER:
    gameoverscreen();
    break;

  case GAME_WON:
    gamewonscreen();
    break;
  }

  // always draw everything
  handleDraw(arduboy);

  // flip the buffer
  arduboy.display();

  // and start all over again
}