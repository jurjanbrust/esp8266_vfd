#include "test.h"
#include "display.h"

TESTDISPLAY::TESTDISPLAY(Display& display)
{
    this->_display = &display;
}

void TESTDISPLAY::write()
{
  this->_display->send("regel en voorbeeld1");
  this->_display->send("regel en voorbeeld2");
  this->_display->send("regel en voorbeeld3");
  this->_display->send("regel en voorbeeld4");
  this->_display->send("regel en voorbeeld5");
  this->_display->send("regel en voorbeeld6 hele labge tekst");
  this->_display->send("regel en voorbeeld7");
  this->_display->send("regel en voorbeeld8");
  this->_display->send("regel en voorbeeld9");
  this->_display->send("regel en voorbeeld10");
  this->_display->send("Jurjan Brust");
  delay(4000);
}