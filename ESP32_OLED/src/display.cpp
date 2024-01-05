#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_SSD1306.h"

#define OLED_SDA 16
#define OLED_SCL 17

Adafruit_SH1106 oleddisplay(OLED_SDA, OLED_SCL);

Display::Display() 
{
}

void Display::begin() {
  oleddisplay.setTextSize(1);
  oleddisplay.setTextColor(WHITE, BLACK);
  oleddisplay.begin(SH1106_SWITCHCAPVCC, 0x3C); 
}

void Display::clear()
{
  oleddisplay.clearDisplay();
}

void Display::display()
{
  oleddisplay.display();
}

void Display::startscrolldiagleft(uint8_t start, uint8_t stop) 
{
  oleddisplay.startscrolldiagleft(start, stop);
  oleddisplay.display();
}

void Display::startscrolldiagright(uint8_t start, uint8_t stop) 
{
  oleddisplay.startscrolldiagright(start, stop);
}

void Display::startscrollleft(uint8_t start, uint8_t stop) 
{
  oleddisplay.startscrollleft(start, stop);
}

void Display::startscrollright(uint8_t start, uint8_t stop) 
{
  oleddisplay.startscrollright(start, stop);
}

void Display::stopscroll() 
{
  oleddisplay.stopscroll();
}

void Display::setCursor(int x, int y) {
  oleddisplay.setCursor(x, y);
}

void Display::linefeed()
{
  oleddisplay.println();
}

void Display::carriagereturn()
{
  oleddisplay.setCursor(0, 17);
}

void Display::enter()
{
  oleddisplay.setCursor(0, 17);
}

void Display::home()
{
  oleddisplay.setCursor(0, 0);
}

void Display::send(String value)
{
  oleddisplay.println(value);
  oleddisplay.display();
}

void Display::setTextSize(uint8_t value)
{
  oleddisplay.setTextSize(value);
}

void Display::send(char value, int ms)
{
  oleddisplay.print(value);
  delay(ms);
}

void Display::fixed(String value)
{
    send(value);
}

void Display::centered(String value)
{
    int width = 20;
    int len = value.length();
    if(width < len) { send(value); }

    int diff = width - len;
    int pad = diff/2;
    return send(String(pad, ' ') + value);
}

void Display::command(const char* value) 
{
  oleddisplay.print(value);
}

void Display::writeBytes(uint8_t data[])
{
  for(int i=0; i<4; i++) {
    oleddisplay.write(data[i]);
  }
}

void Display::writeByte(uint8_t data)
{
    oleddisplay.write(data);
}

void Display::writeAndDelay(uint8_t data[], int ms) 
{
  writeBytes(data);
  delay(ms);
}
