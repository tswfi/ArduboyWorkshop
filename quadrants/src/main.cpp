#include <Arduino.h>
#include <Arduboy2.h>

Arduboy2 arduboy;

uint16_t ballx = WIDTH / 2;
uint16_t bally = HEIGHT / 2;
const uint8_t ballr = 2;

uint16_t boxx = 20;
uint16_t boxy = 20;
uint16_t boxwidth = 20;
uint16_t boxheight = 10;

void setup()
{
  arduboy.begin();
}

void loop()
{
  if (!arduboy.nextFrame())
    return;
  arduboy.clear();

  // move ball if there is no collision between the ball and the box
  switch (arduboy.buttonsState())
  {
  case LEFT_BUTTON:
    ballx--;
    break;
  case RIGHT_BUTTON:
    ballx++;
    break;
  case DOWN_BUTTON:
    bally++;
    break;
  case UP_BUTTON:
    bally--;
    break;

  default:
    break;
  }


  // calculate quadrants
  int16_t cx = abs(ballx - boxx) - boxwidth / 2;
  int16_t cy = abs(bally - boxy) - boxheight / 2;

  arduboy.setCursor(0, 0);
  arduboy.print(cx);
  arduboy.print(":");
  arduboy.print(cy);

  // draw box
  arduboy.fillRect(boxx, boxy, boxwidth, boxheight);
  // draw ball
  arduboy.fillCircle(ballx, bally, ballr);
  arduboy.display();
}