#ifndef VFD_h
#define VFD_h

#include <inttypes.h>
#include "Print.h"
#include <string>

#define vfd_bellAlarmOutput "\x07"
#define vfd_backSpaceCursor "\x08"
#define vfd_advanceCursor "\x09"
#define vfd_lineFeed "\x0A"
#define vfd_blinkBlockCursor "\x0B"
#define vfd_underbarCursor "\x0C"
#define vfd_carriageReturn "\x0D"
#define vfd_cursorOff "\x0E"
#define vfd_cursorOn "\x0F"
#define vfd_scrollLineLock const char* "\x10"
#define vfd_setVerticalScrollMode "\x11"
#define vfd_SetHorizontalScrollMode "\x13"
#define vfd_softwareReset "\x14"
#define vfd_clearDisplayAndHomeCursor "\x15"
#define vfd_homeCursor "\x16"
#define vfd_setDataBit7High "\x17"
#define vfd_beginUserDefinedCharacter "\x18"
#define vfd_setAddressBit0High "\x19"
#define vfd_cursorUpOneLine "\x1A"
#define vfd_moveCursorToDesignatedLocation "\x1B"
#define vfd_setEuropeanCharacterSet "\x1C"
#define vfd_setKatakanaCharacterSet "\x1D"
#define vfd_setCryllicCharacterSet "\x1E"
#define vfd_setHebrewCharacterSet "\x1F"
#define vfd_setDisplayScreenOrBrightness "\x30"
#define vfd_beginBlinkingCharacters "\x31"
#define vfd_endBlinkingCharacters "\x32"
#define vfd_blankDisplayScreen "\x33"
#define vfd_unblankDisplayScreen "\x34"
#define vfd_commaPeriodTriangleFunction "\x35"
#define vfd_eraseLineDataWithEndBlink "\x36"
#define vfd_setCarriageReturnAndLineFeedDefinitions "\x37"
#define vfd_underbarOn "\x38"
#define vfd_underbarOff "\x39"
#define vfd_selectRightToLeftDataEntry "\x3A"
#define vfd_selectLeftToRightDataEntry "\x3B"
#define vfd_screenSaverOn "\x3V"
#define vfd_screenSaverOff "\x3D"
#define vfd_executeSelfTest "\x3E"
#define vfd_terminateSelfTest "\x3F"

class VFD {

public:
  VFD();
  void begin();
  void send(String);
  void send(char value, int ms);
  void writeBytes(uint8_t data[]);
  void writeByte(uint8_t data);
  void writeAndDelay(uint8_t *, int);
  void command(const char *);
  void typeWriteHorizontal(String);
  void typeWriteVertical(String);
  void fixed(String);
  void clear();
  void linefeed();
  void carriagereturn();
  void home();
  void enter();
  void fadeOut();
  void fadeIn();
  void knightRider();
  void knightRiderMode2();
private:
  // 0x19 = command operation
  // 0x30 = set level
  // 0xFF = columnid from 0x00 to 0x19 > Column ID code 0xFF sets all columns. 
  uint8_t brightness0[4] = { 0x19, 0x30, 0xFF, 0x00 }; // brightest
  uint8_t brightness1[4] = { 0x19, 0x30, 0xFF, 0x01 };
  uint8_t brightness2[4] = { 0x19, 0x30, 0xFF, 0x02 };
  uint8_t brightness3[4] = { 0x19, 0x30, 0xFF, 0x03 };
  uint8_t brightness4[4] = { 0x19, 0x30, 0xFF, 0x04 };
  uint8_t brightness5[4] = { 0x19, 0x30, 0xFF, 0x05 };
  uint8_t brightness6[4] = { 0x19, 0x30, 0xFF, 0x06 };
  uint8_t brightness7[4] = { 0x19, 0x30, 0xFF, 0x07 }; // dimmest
  uint8_t crlf[2] = { 0x0D, 0x0A };
};

#endif