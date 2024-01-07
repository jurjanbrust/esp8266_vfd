#pragma once

#include "Print.h"

class Display {

public:
  Display();
  void begin();
  void send(String);
  void send(char value, int ms);
  void writeBytes(uint8_t data[]);
  void writeByte(uint8_t data);
  void writeAndDelay(uint8_t *, int);
  void setBacklight(bool on);
  void command(const char *);
  void fixed(String);
  void centered(String);
  void clear();
  void linefeed();
  void carriagereturn();
  void home();
  void scrollMessage(int row, String message, int delayTime, int totalColumns);
  void enter();
};
