#include "PinoutDigital.h"




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




PinoutDigital::PinoutDigital()
{
}


PinoutDigital::PinoutDigital(byte pin, const prog_char* label, const boolean isNC)
{
  this->init(pin, label);
  this->_id.isNC = isNC;
}


PinoutDigital::PinoutDigital(byte pin, const prog_char* label, const boolean isNC, const boolean value)
{
  this->init(pin, label);
  this->_id.isNC      = isNC;
  this->_id.isActive  = value;
}


PinoutDigital::~PinoutDigital()
{
}