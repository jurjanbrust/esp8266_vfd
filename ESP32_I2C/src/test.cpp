#include "test.h"
#include "display.h"

TESTDISPLAY::TESTDISPLAY(Display& display)
{
    this->_display = &display;
    this->_display->clear();
}

void TESTDISPLAY::start()
{
    write();
}

void TESTDISPLAY::write()
{
    this->_display->send("regel en voorbeeld");
}