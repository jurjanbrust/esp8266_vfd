#include "test.h"
#include "vfd.h"

TESTDISPLAY::TESTDISPLAY(VFD& vfd)
{
    this->_vfd = &vfd;
    this->_vfd->clear();
    this->_vfd->command(vfd_cursorOff);
}

void TESTDISPLAY::start()
{
    //scrollText();
    knightRider();
}

void TESTDISPLAY::scrollText()
{
    this->_vfd->typeWriteVertical("regel 1 en die is ook best lang, maar dat gaan dan zo door verticaal en dan komt dit tot een einde");
    this->_vfd->enter();
    this->_vfd->typeWriteHorizontal("regel 2 en die is heel erg lang maar daar scrollen we dan horizontaal want dat kan ook natuurlijk gewoon op dat ding, is het niet mooi");
    this->_vfd->fadeOut();
    delay(1000);
    this->_vfd->clear();
}

void TESTDISPLAY::knightRider()
{
    this->_vfd->send("regel en voorbeeld");
    this->_vfd->enter();
    this->_vfd->send("knightrider mode !!");
    this->_vfd->knightRider();
    delay(1000);
    this->_vfd->home();
}