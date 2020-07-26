---
title: Hello world
nav: false
---



# Hello World main.cpp

```cpp
#include <Arduino.h>
#include <Arduboy2.h>

// instantiate our arduboy object
Arduboy2 arduboy;

// variables
uint8_t y = 0; // text y coordinate

// setup is called first
void setup()
{
  // begin, as per arduino guidelines
  arduboy.begin();
  // clear screen
  arduboy.clear();
  // print our text to the buffer
  arduboy.print("Hello World");
  // and display the buffer
  arduboy.display();
}

// loop is called all the time again and again and again
void loop()
{
  // arduboy has some helper methods for making our lives easier.
  // nextFrame tries to keep everything at 60hz
  // if we are not yet there just return and check again
  if (!arduboy.nextFrame())
    return;

  // another nice helper, every 30 frames we will do our logic here
  if (arduboy.everyXFrames(30))
  {
    // arduboy knows the screen height, lets keep our variable inside the screen
    if (y > arduboy.height())
    {
      y = 0;
    }
    else
    {
      y++;
    }

    // clear the screen again
    arduboy.clear();
    // set cursor in the position we calculated
    arduboy.setCursor(0, y);
    // print the text again to the buffer
    arduboy.print("Hello World");
    // and display it
    arduboy.display();

    // and blink our led
    arduboy.setRGBled(y, 0, 0);
  }
}

```
