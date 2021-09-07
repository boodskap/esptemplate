#include "DummyPrinter.h"

DummyPrinter VoidPrinter;

size_t DummyPrinter::write(uint8_t)
{
  return 0;
}