#ifndef _DUMMY_PRINTER_H_
#define _DUMMY_PRINTER_H_

#include <Arduino.h>

class DummyPrinter : public Print
{

public:

     virtual size_t write(uint8_t);

};

extern DummyPrinter VoidPrinter;

#endif //_DUMMY_PRINTER_H_