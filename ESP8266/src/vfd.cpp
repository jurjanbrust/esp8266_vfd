#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "vfd.h"

VFD::VFD() 
{
}

void VFD::begin() {
  clear();
}

void VFD::clear()
{
  command(vfd_clearDisplayAndHomeCursor);  // clear display, set cursor position to zero
}

/*
* Moves one line down at the current cursor position, for example
* Line 1
*       Line2 cursor will be here
*/
void VFD::linefeed()
{
  command(vfd_lineFeed);
}

void VFD::carriagereturn()
{
  command(vfd_carriageReturn);
}

void VFD::home()
{
  command(vfd_homeCursor);  // set cursor position to zero
}

void VFD::send(String value)
{
  Serial.print(value);
  delay(100);
}

void VFD::send(char value, int ms)
{
  Serial.print(value);
  delay(ms);
}

void VFD::fixed(String value)
{
    value = value + "                      ";
    value = value.substring(0, 20);
    send(value);
}

void VFD::centered(String value)
{
    int width = 20;
    int len = value.length();
    if(width < len) { send(value); }

    int diff = width - len;
    int pad = diff/2;
    return send(String(pad, ' ') + value);
}

void VFD::command(const char* value) 
{
  Serial.write(value);
}

void VFD::writeBytes(uint8_t data[])
{
  for(int i=0; i<4; i++) {
    Serial.write(data[i]);
  }
}

void VFD::typeWriteVertical(String line)
{
  command(vfd_cursorOn);
  command(vfd_setVerticalScrollMode);
  for(unsigned int i=0; i<line.length();i++) {
    send(line.charAt(i),70);
  }
  command(vfd_cursorOff);
}

void VFD::typeWriteHorizontal(String line)
{
  command(vfd_cursorOn);
  command(vfd_SetHorizontalScrollMode);
  for(unsigned int i=0; i<line.length();i++) {
    send(line.charAt(i),70);
  }
  command(vfd_cursorOff);
}

void VFD::writeByte(uint8_t data)
{
    Serial.write(data);
}

void VFD::enter()
{
  writeBytes(crlf);
}

void VFD::writeAndDelay(uint8_t data[], int ms) 
{
  writeBytes(data);
  delay(ms);
}

void VFD::setBrightness0()
{
  writeBytes(brightness0);
}

void VFD::setBrightness1()
{
  writeBytes(brightness1);
}

void VFD::setBrightness2()
{
  writeBytes(brightness2);
}

void VFD::setBrightness3()
{
  writeBytes(brightness3);
}

void VFD::setBrightness4()
{
  writeBytes(brightness4);
}

void VFD::setBrightness5()
{
  writeBytes(brightness5);
}

void VFD::setBrightness6()
{
  writeBytes(brightness6);
}

void VFD::setBrightness7()
{
  writeBytes(brightness7);
}

void VFD::fadeIn()
{
  int ms = 100;
  writeAndDelay(brightness7, ms);
  writeAndDelay(brightness6, ms);
  writeAndDelay(brightness5, ms);
  writeAndDelay(brightness4, ms);
  writeAndDelay(brightness3, ms);
  writeAndDelay(brightness2, ms);
  writeAndDelay(brightness1, ms);
  writeAndDelay(brightness0, ms);
}

void VFD::fadeOut()
{
  int ms = 100;
  writeAndDelay(brightness0, ms);
  writeAndDelay(brightness1, ms);
  writeAndDelay(brightness2, ms);
  writeAndDelay(brightness3, ms);
  writeAndDelay(brightness4, ms);
  writeAndDelay(brightness5, ms);
  writeAndDelay(brightness6, ms);
  writeAndDelay(brightness7, ms);
}

void VFD::knightRider()
{
  const int positionindex = 2; // index of array that contains the position
  const int levelindex = 3; // index of array that contains the brightness
  uint8_t brightness[4]  = { 0x19, 0x30, 0x00, 0x00 };
  int speed = 50;
  fadeOut();
  delay(50);

  // van links naar rechts
  for (int index = 0; index < 20; index++)
  {
      int level = 7 - index;
      brightness[positionindex] = (uint8_t)index;
      if(level > 7) {
        level=7;
      }
      if(level < 0) {
        level = 0;
      }
      brightness[levelindex] = (uint8_t)level;
      writeAndDelay(brightness, speed);
  }

  for (int index = 19; index >= 0; index--)
  {
      int level = index;
      brightness[positionindex] = (uint8_t)index;
      if(level > 7) {
        level=7;
      }
      brightness[levelindex] = (uint8_t)level;
      writeAndDelay(brightness, speed);
  }
  //delay(50);
  fadeIn();
}

void VFD::knightRider2()
{
  const int positionindex = 2; // index of array that contains the position
  const int levelindex = 3; // index of array that contains the brightness
  uint8_t brightness[4]  = { 0x19, 0x30, 0x00, 0x00 };
  int speed = 50;
  fadeIn();
  delay(100);

  // van links naar rechts
  for (int index = 0; index < 20; index++)
  {
      int level = 0 + index;
      brightness[positionindex] = (uint8_t)index;
      if(level > 7) {
        level=7;
      }
      if(level < 0) {
        level = 0;
      }
      brightness[levelindex] = (uint8_t)level;
      writeAndDelay(brightness, speed);
  }

  for (int index = 19; index >= 0; index--)
  {
      int level = index;
      brightness[positionindex] = (uint8_t)index;
      if(level > 7) {
        level=7;
      }
      brightness[levelindex] = (uint8_t)level;
      writeAndDelay(brightness, speed);
  }
  //delay(50);
  fadeIn();
}

void VFD::fadeLeftToRight()
{
  const int positionindex = 2; // index of array that contains the position
  const int levelindex = 3; // index of array that contains the brightness

  uint8_t brightness[4]  = { 0x19, 0x30, 0x00, 0x00 };
  uint8_t level[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int speed = 10;
  
  // van links naar rechts
  for (int i = 0; i < 20; i++)
  {
      for (int j = 0 ; j < 20; j++)
      {
        if(level[j] < 7) {
          level[j]++;       
        }
        brightness[positionindex] = (uint8_t)j;
        brightness[levelindex] = (uint8_t)level[j];
        writeAndDelay(brightness, speed);
      }
  }
}

void VFD::fadeRightToLeft()
{
  const int positionindex = 2; // index of array that contains the position
  const int levelindex = 3; // index of array that contains the brightness

  uint8_t brightness[4]  = { 0x19, 0x30, 0x00, 0x00 };
  uint8_t level[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int speed = 50;
  
  // van rechts naar links
  for (int i = 19; i >= 0; i--)
  {
      for (int j = 19; j >=0; j--)
      {
        if(level[j] > 0) {
          level[j]--;       
        }
        brightness[positionindex] = (uint8_t)j;
        brightness[levelindex] = (uint8_t)level[j];
        writeAndDelay(brightness, speed);
      }
  }
}

void VFD::knightRider3()
{
  const int positionindex = 2; // index of array that contains the position
  const int levelindex = 3; // index of array that contains the brightness
  uint8_t combined[32]  = { 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00, 0x19, 0x30, 0x19, 0x00 };
  int speed = 200;

  fadeOut();
  delay(2000);

  for (int index = 0; index < 20; index++)
  {
      combined[positionindex+(0*4)] = (uint8_t)index;
      combined[levelindex+(0*4)] = (uint8_t)0;
      if (index > 0)
      {
          combined[positionindex+(1*4)] = (uint8_t)(index - 1);
          combined[levelindex+(1*4)] = (uint8_t)1;
      }
      if (index > 1)
      {
          combined[positionindex+(2*4)] = (uint8_t)(index - 2);
          combined[levelindex+(2*4)] = (uint8_t)2;
      }
      if (index > 2)
      {
          combined[positionindex+(3*4)] = (uint8_t)(index - 3);
          combined[levelindex+(3*4)] = (uint8_t)3;
      }
      if (index > 3)
      {
          combined[positionindex+(4*4)] = (uint8_t)(index - 4);
          combined[levelindex+(4*4)] = (uint8_t)4;
      }
      if (index > 4)
      {
          combined[positionindex+(5*4)] = (uint8_t)(index - 5);
          combined[levelindex+(5*4)] = (uint8_t)5;
      }
      if (index > 5)
      {
          combined[positionindex+(6*4)] = (uint8_t)(index - 6);
          combined[levelindex+(6*4)] = (uint8_t)6;
      }
      if (index > 6)
      {
          combined[positionindex+(7*4)] = (uint8_t)(index - 7);
          combined[levelindex+(7*4)] = (uint8_t)7;
      }
      Serial.write(index);
      writeAndDelay(combined, speed);
  }
}