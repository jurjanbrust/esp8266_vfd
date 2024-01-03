#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "display.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16,2);

Display::Display() 
{
    //lcd.init();
    //   // Turn on the backlight on LCD. 
    // // print the Message on the LCD. 
    // lcd.print("Hello world.");
}

void Display::begin() {
  lcd.init();
  lcd.setBacklight(HIGH);
}

void Display::clear()
{
  lcd.clear();
}

void Display::linefeed()
{
  lcd.setCursor(0, 1);
}

void Display::carriagereturn()
{
  lcd.setCursor(0, 1);
}

void Display::enter()
{
  lcd.setCursor(0, 1);
}

void Display::home()
{
  lcd.setCursor(0, 0);
}

void Display::send(String value)
{
  lcd.print(value);
  delay(100);
}

void Display::send(char value, int ms)
{
  Serial.print(value);
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
  lcd.print(value);
}

void Display::writeBytes(uint8_t data[])
{
  for(int i=0; i<4; i++) {
    lcd.write(data[i]);
  }
}

void Display::writeByte(uint8_t data)
{
    lcd.write(data);
}

void Display::writeAndDelay(uint8_t data[], int ms) 
{
  writeBytes(data);
  delay(ms);
}
