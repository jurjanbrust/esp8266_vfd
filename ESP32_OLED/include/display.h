#pragma once

#include "Print.h"

class Display {

public:
  Display();
  void begin();
  void send(String);
  void setTextSize(uint8_t value);
  void send(char value, int ms);
  void writeBytes(uint8_t data[]);
  void writeByte(uint8_t data);
  void writeAndDelay(uint8_t *, int);
  void command(const char *);
  void fixed(String);
  void centered(String);
  void clear();
  void display();
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);
  void startscrollright(uint8_t start, uint8_t stop);
  void stopscroll();
  void setCursor(int x, int y);
  void linefeed();
  void carriagereturn();
  void home();
  void enter();
};
