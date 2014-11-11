#include "PinoutPulse.h"




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




PinoutPulse::PinoutPulse()
{
}


PinoutPulse::PinoutPulse(const byte pin, const prog_char* label)
{
  this->init(pin, label);
  this->_value = 0;
}


PinoutPulse::PinoutPulse(const byte pin, const prog_char* label, uint8_t value)
{
  this->init(pin, label);
  this->_value = value;
}


PinoutPulse::~PinoutPulse()
{
}


void PinoutPulse::addValue(const uint8_t d)
{
  this->_value = constrain(this->getValue() + d, 0, (uint8_t) -1);
  analogWrite(this->getPin(), this->getValue());
}